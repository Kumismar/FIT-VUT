#include "headers/argument_processing.h"

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

bool isValidPort(int portNumber)
{
    return (portNumber >= 0 && portNumber <= MAX_PORT_NUM);
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

bool isUsedArg(char arg)
{
    static char arguments[4];
    memset(arguments, '\0', sizeof(arguments));
    for (int i = 0; i < 3; i++)
    {
        if (arg == arguments[i])
            return true;
    }

    int i = 0;
    while (arguments[++i] != '\0');
    arguments[i] = arg;

    return false;
}

void processCurrentArgument(char arg, char **argv, Arguments *args, int *i)
{
    switch(arg)
    {
        case 'm':
            args->mode = getMode(argv[++(*i)]); 
            break;

        case 'h':
            (*i)++;
            args->host = (char*)mallocAndAddToList(strlen(argv[*i]) + 1);
            strcpy(args->host, argv[*i]);
            break;

        case 'p':
            args->port = getPort(argv[++(*i)]);
            break;

        default:
            errorExit("processCurrentArgument switch program failure\n", PROGRAM_FAILURE);
    }
}

void processArguments(int argc, char **argv, Arguments *args)
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printHelp();
            disposeList();
            exit(EXIT_SUCCESS);
        }
    }
    else if (argc != 7)
        errorExit("Invalid number of command-line arguments.\n", INVALID_ARG_NUM);

    for (int i = 1; i < argc; i++)
    {
        bool isValidArgument = (strcmp(argv[i], "-m") == 0) || \
                               (strcmp(argv[i], "-p") == 0) || \
                               (strcmp(argv[i], "-h") == 0);

        if (isValidArgument && !isUsedArg(argv[i][1]))
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