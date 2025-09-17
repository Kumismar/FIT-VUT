#ifndef error_functions_h
#define error_functions_h

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "list.h"

/**
 * @brief Prints given string to stderr and exits with given constant.
 * 
 * @param string Error to be written to stderr.
 * @param exitCode Error code to be returned from the program.
 */
void errorExit(const char *string, int exitCode);

/**
 * @brief Prints basic usage of the program to stdout whenever the program is started 
 * with wrong command-line arguments.
 */
void printHelp();

#endif /* error_functions_h */