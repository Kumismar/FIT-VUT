#ifndef error_functions_h
#define error_functions_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "errors.h"
#include "pointerList.h"

#define DEBUG 0

/**
 * @brief Prints given string to *stderr*, disposes list of pointers to allocated memory and exits with given constant.
 * 
 * @param string Error to be written to stderr.
 * @param exitCode Error code to be returned from the program.
 */
void errorExit(const char *string, int exitCode);

/**
 * @brief Prints basic usage of the program to stdout whenever the program is started 
 * with wrong command-line arguments or `--help` option is present.
 */
void printHelp();

/**
 * @brief Auxiliary function for debugging the program when under development.
 * Works the same as `printf()` function but prints only when constant **DEBUG** is 1.
 */
void debugMessage(const char *string, ...);

#endif /* error_functions_h */