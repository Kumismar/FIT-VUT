#include "../headers/error_functions.h"

void errorExit(const char *string, int exitCode)
{
    if (allocMemList != NULL)
        disposeList();

    fprintf(stderr, "%s\n", string);
    exit(exitCode);
}

void printHelp()
{
    printf("Usage: ./ipkcpc [OPTIONS VALUES] | --help\n" 
           "--help: Shows this help.\n"
           "OPTIONS:\n"
           "    -m, VALUE: (TCP|UDP|tcp|udp)\n"
           "    -p, VALUE: [0 - 65535]\n"
           "    -h, VALUE: IPv4 address - [0-255].[0-255].[0-255].[0-255]\n"
           "Every option has to be present only once. The order doesn't matter.\n\n");
}