#include "headers/ipkcpd.h"

static volatile bool running = true;

static void interruptHandler()
{
    running = false;
}

int openSocket(Modes mode)
{
    int connectionType;
    if (mode == TCP) 
        connectionType = SOCK_STREAM;
    else 
        connectionType = SOCK_DGRAM;

    int clientSocket = socket(AF_INET, connectionType, 0);
    if (clientSocket == -1)
        errorExit("Failed to open file descriptor for socket.\n", ERR_SOCKET);

    return clientSocket;
}

void bindAddressToSocket(int socketFd, socklen_t addrSize, struct sockaddr *address)
{
    int check = bind(socketFd, address, addrSize);
    if (check != 0)
        errorExit("Bind failed.\n", ERR_BIND);
}

in_addr_t getAddress(char *IPAddr)
{
    in_addr_t ipv4;
    int check = inet_pton(AF_INET, IPAddr, &ipv4);
    if (check != 1) 
    {
        printHelp();
        errorExit("Invalid IPv4 address.\n", INVALID_IPV4);
    }
    return ipv4;
}

void communicateWithUDP(int socketFd, char *buffer, struct sockaddr *address, socklen_t addressSize)
{
    while (running)
    {
        int bytesRecieved = recvfrom(socketFd, buffer, MAX_BUFFER_LENGTH, 0, address, &addressSize);
        if (bytesRecieved < 0)
        {   
            perror("recvfrom");
            errorExit("Recvfrom failed.\n", ERR_RECVFROM);
        }
        else if (bytesRecieved == 0)
            break;

        debugMessage("RECIEVED: %s", buffer + SKIP_OPCODE_MSGLENGTH);
        buffer = parseRecievedQuery(buffer, getResponseMessageUDP, UDP);

        int bufferLength = strlen(buffer + OPCODE_STATUS_LENGTH) + OPCODE_STATUS_LENGTH;

        debugMessage("SENDING: %s", buffer + OPCODE_STATUS_LENGTH);
        debugMessage("LENGTH:%d\n", bufferLength);

        int bytesSent = sendto(socketFd, buffer, bufferLength, 0, address, addressSize);
        if (bytesSent < 0)
        {
            perror("sendto");
            errorExit("Sendto failed.\n", ERR_SENDTO);
        }
    }
    close(socketFd);
}

void childCommunicationTCP(int communicationSocket, char *buffer)
{
    int helloCounter = 0;
    while (1)
    {
        memset(buffer, 0, MAX_BUFFER_LENGTH);
        int bytesRecieved = recv(communicationSocket, buffer, MAX_BUFFER_LENGTH, 0);
        if (bytesRecieved < 0)
            break;

        debugMessage("RECIEVED: %s", buffer);
        
        if (strcmp(buffer, "HELLO\n") == 0)
        {
            helloCounter++;
            memset(buffer, 0, MAX_BUFFER_LENGTH);
            if (helloCounter > 1)
                strcpy(buffer, "BYE\n");
            else
                strcpy(buffer, "HELLO\n");
        }
        else if (strcmp(buffer, "BYE\n") == 0 || 
                (strcmp(buffer, "HELLO\n") != 0 && helloCounter < 1))
        {
            memset(buffer, 0, MAX_BUFFER_LENGTH);
            strcpy(buffer, "BYE\n");
        }
        else
            parseRecievedQuery(buffer, getResponseMessageTCP, TCP);
        
        int bytesSent = send(communicationSocket, buffer, strlen(buffer), 0);    
        if (bytesSent < 0)
            break;

        if (strcmp(buffer, "BYE\n") == 0)
            break;
    }
    debugMessage("child is ded\n");
    close(communicationSocket);
    exit(EXIT_SUCCESS);
}

void communicateWithTCP(int welcomeSocket, char *buffer, struct sockaddr *address, socklen_t addressSize)
{
    if (listen(welcomeSocket, MAX_CONNECTIONS) < 0)
        errorExit("Listen failed.\n", ERR_LISTEN);
        
    while (running)
    {
        int communicationSocket = accept(welcomeSocket, address, &addressSize);
        if (communicationSocket <= 0)
            continue;

        pid_t pid = fork();
        if (pid < 0)
            errorExit("Fork failed\n", ERR_FORK);

        if (pid == 0)
            childCommunicationTCP(communicationSocket, buffer);
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, interruptHandler);
    
    initList();

    Arguments args;
    processArguments(argc, argv, &args);
    
    struct sockaddr_in addressInfo;
    addressInfo.sin_family = AF_INET;
    addressInfo.sin_port = htons(args.port);
    addressInfo.sin_addr.s_addr = getAddress(args.host);

    struct sockaddr *address = (struct sockaddr*) &addressInfo;
    socklen_t addressSize = sizeof(addressInfo); 

    char *buffer = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH); 
    memset(buffer, 0, MAX_BUFFER_LENGTH);

    int socketFd = openSocket(args.mode);
    debugMessage("%d\n", socketFd);
    bindAddressToSocket(socketFd, addressSize, address);

    if (args.mode == UDP)
        communicateWithUDP(socketFd, buffer, address, addressSize);
    else
        communicateWithTCP(socketFd, buffer, address, addressSize);

    freeAndRemoveFromList(buffer);
    freeAndRemoveFromList(args.host);
    disposeList();
    return 0;
}

