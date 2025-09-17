#include "../headers/argument_processing.h"

bool strtolFailed(char *stringPtr, char *endPtr)
{
    if (stringPtr == endPtr)
        return true;
    else if (*endPtr != '\0')
        return true;

    return false;
}

Modes getMode(char* mode)
{
    if ((strcmp(mode, "udp") == 0) || \
        (strcmp(mode, "UDP") == 0))
        return UDP;
    else if ((strcmp(mode, "tcp") == 0) || \
             (strcmp(mode, "TCP") == 0))
        return TCP;
    else
    {
        printHelp();
        errorExit("Invalid mode.\n", INVALID_MODE);
        /* Compiler warnings were thrown for reaching end of nonvoid function. */
        return TCP;
    }
}

bool isValidByte(long byte)
{
    return (byte >= 0 && byte <= __UINT8_MAX__);
}

char* parseIPAddress(char* IPAddr)
{
    const char delimiter[] = ".";
    /* Strtol returns long int. */
    long IPAddrByteValue; 
    int byteCount = 0;
    int IPAddrLength = strlen(IPAddr);

    /* Strtok breaks the original string. */
    char *IPAddrCopy = (char*)mallocAndAddToList(IPAddrLength + 1);
    strcpy(IPAddrCopy, IPAddr);

    /* Strtol stores the rest of string in it's second argument. Logically,
       in worst-case scenario, the longest it gets is the whole input string. */
    char *remainder = (char*)mallocAndAddToList(IPAddrLength + 1);

    /* The address of remainder can be changed by strtol and freeing it is not safe nor easy.
       Saving the original address in another pointer is the easiest workaround. */
    char *oldPtr = remainder; 

    for (char* IPAddrByte = strtok(IPAddrCopy, delimiter); IPAddrByte != NULL; IPAddrByte = strtok(NULL, delimiter))
    {
        byteCount++;
        IPAddrByteValue = strtol(IPAddrByte, &remainder, 10);

        if (strtolFailed(IPAddrByte, remainder))
        {
            printHelp();
            errorExit("Invalid IPv4 address format.\n", INVALID_IPV4);
        }
        else if (!isValidByte(IPAddrByteValue))
        {
            printHelp();
            errorExit("Invalid IPv4 address format.\n", INVALID_IPV4);
        }
    }

    /* This piece of code is a bit specific; the '.' is used as delimiter in strtok,
       so it isn't detected when present at the end of the string. */
    if (IPAddr[IPAddrLength - 1] == '.')
    {
        printHelp();
        errorExit("Invalid IPv4 address format.\n", INVALID_IPV4);
    }

    if (byteCount != 4)
    {
        printHelp();
        errorExit("Invalid IPv4 address format.\n", INVALID_IPV4);
    }

    freeAndRemoveFromList(IPAddrCopy);
    freeAndRemoveFromList(oldPtr);
    return IPAddr;
}

bool isValidPort(int portNumber)
{
    return (portNumber >= 0 && portNumber <= __UINT16_MAX__);
}

int getPort(char* port)
{
    char *remainder = NULL;
    int portNumber = strtol(port, &remainder, 10);

    if (strtolFailed(port, remainder))
        errorExit("Invalid port format.\n", INVALID_PORT);
    
    if (!isValidPort(portNumber))
        errorExit("Invalid port format.\n", INVALID_PORT);
    
    return portNumber;
}

void processCurrentArgument(char arg, char **argv, Arguments *args, int *i)
{
    switch(arg)
    {
        case 'm':
            args->mode = getMode(argv[*i + 1]);
            (*i)++; 
            break;

        case 'h':
            args->host = parseIPAddress(argv[*i + 1]);
            (*i)++;
            break;

        case 'p':
            args->port = getPort(argv[*i + 1]);
            (*i)++;
            break;

        default:
            errorExit("The program should never get here.\n", PROGRAM_FAILURE);
    }
}

bool isUsedArg(char *arg)
{
    static char arguments[3][3];
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(arg, arguments[i]) == 0)
            return true;
    }
    static int j = 0;
    strcpy(arguments[j], arg);
    j++;
    return false;
}

void processArguments(int argc, char **argv, Arguments *args)
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        printHelp();
        disposeList();
        exit(EXIT_SUCCESS);
    }

    if (argc != 7)
    {
        printHelp();
        errorExit("Invalid number of arguments.\n", INVALID_ARG_NUM);
    }

    for (int i = 1; i < argc; i++)
    {
        bool isValidArgument = (strcmp(argv[i], "-m") == 0) || \
                               (strcmp(argv[i], "-p") == 0) || \
                               (strcmp(argv[i], "-h") == 0);
                               
        if (isValidArgument && !isUsedArg(argv[i]))
        {
            /* Skipping the '-' character so switch can be used just for the characters (m, h, p). */
            char currentArgument = argv[i][1];
            processCurrentArgument(currentArgument, argv, args, &i);
        }
        else
        {
            printHelp();
            errorExit("Wrong argument.\n", INVALID_CMD_ARGS);
        }
    }
}