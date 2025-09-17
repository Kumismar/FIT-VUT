#include "../headers/ipkcpc.h"

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
        errorExit("Failed to open file descriptor for socket.\n", SOCKET_FD_ERROR);

    return clientSocket;
}

in_addr_t getAddress(char *address)
{
    in_addr_t ipv4;
    int check = inet_pton(AF_INET, address, &ipv4);

    /* This should be always false because the validity is already checked
       and the implementation tested. */
    if (check == 0)
        errorExit("Invalid IPv4 address format.\n", INVALID_IPV4);
    return ipv4;
}

void communicateWithUDP(struct sockaddr* address, socklen_t addressSize, char* buffer, size_t maxBufferLength)
{
    int clientSocket = openSocket(UDP);

    /* Skipping opcode, payload data length (and status) bytes to get straight to data */
    char *payloadData = buffer + SKIP_OPCODE_AND_LENGTH;
    char *payloadDataRecieved = buffer + SKIP_OPCODE_STATUS_AND_LENGTH; 

    int payloadDataLength = maxBufferLength - OPCODE_AND_LENGTH;
    
    memset(buffer, '\0', maxBufferLength);

    while (fgets(payloadData, payloadDataLength, stdin) != NULL && running == true)
    {
        /* Filling the first two bytes with opcode and payload length */
        buffer[0] = 0;
        buffer[1] = strlen(payloadData);

        /* Sending data to the server */
        int bytesSent = sendto(clientSocket, buffer, strlen(payloadData) + OPCODE_AND_LENGTH, 0, address, addressSize);
        if (bytesSent < 0)
            errorExit("Sendto failed.\n", SENDTO_FAILED);

        memset(buffer, '\0', maxBufferLength);

        /* Waiting for server response */
        int bytesRecieved = recvfrom(clientSocket, buffer, maxBufferLength, 0, address, &addressSize);
        if (bytesRecieved < 0)
            errorExit("Recvfrom failed.\n", RECVFROM_FAILED);

        /* Print recieved data in correct format */
        if (buffer[1] == 0)
            printf("OK:%s", payloadDataRecieved);
        else
            printf("ERR:%s", payloadDataRecieved);

        memset(buffer, '\0', maxBufferLength);
    }
    close(clientSocket);
}

void communicateWithTCP(struct sockaddr* address, socklen_t addressSize, char* buffer, size_t maxBufferLength)
{
    /* Opening a socket and connecting to remote */
    int clientSocket = openSocket(TCP);
    if (connect(clientSocket, address, addressSize) != 0)
        errorExit("Connect failed.\n", CONNECT_FAILED);

    memset(buffer, '\0', maxBufferLength);

    while (fgets(buffer, maxBufferLength, stdin) != NULL && running == true)
    {
        /* Sending data to a connected remote */
        int bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent < 0)
            errorExit("Send failed.\n", SENDTO_FAILED);
        
        memset(buffer, '\0', maxBufferLength);

        /* Waiting for server response */
        int bytesRecieved = recv(clientSocket, buffer, maxBufferLength, 0);
        if (bytesRecieved < 0)
            errorExit("Recv failed.\n", RECVFROM_FAILED);
        
        /* Printing out message from server */
        printf("%s", buffer);

        /* Ending connection if the server responded BYE */
        if (strcmp(buffer, "BYE") == 0)
        {
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
            return;
        }

        memset(buffer, '\0', maxBufferLength);
    }
    if (!running)
    {
        int bytesSent = send(clientSocket, "BYE\n", strlen("BYE\n"), 0);
        if (bytesSent < 0)
            errorExit("Send failed.\n", SENDTO_FAILED);

        int bytesRecieved = recv(clientSocket, buffer, maxBufferLength, 0);
        if (bytesRecieved < 0)
            errorExit("Recv failed.\n", RECVFROM_FAILED);

        printf("\n%s", buffer);
    }

    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
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

    size_t maxBufferLength = 256;
    char* buffer = (char*)mallocAndAddToList(maxBufferLength);

    if (args.mode == UDP)
        communicateWithUDP(address, addressSize, buffer, maxBufferLength);
    else
        communicateWithTCP(address, addressSize, buffer, maxBufferLength);

    freeAndRemoveFromList(buffer);
    disposeList();
    return EXIT_SUCCESS;
}
