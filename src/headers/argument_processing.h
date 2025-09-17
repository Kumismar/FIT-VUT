#ifndef argument_processing_h
#define argument_processing_h

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "error_functions.h"
#include "pointerList.h"

#define IPADDR_BYTE_MAX __UINT8_MAX__
#define MAX_PORT_NUM __UINT16_MAX__

/**
 * @brief Enum containing values TCP and UDP.
 * 
 * Helps to store mode in which the server will communicate.
 * Makes working with modes easier.
 */
typedef enum modes
{
    TCP, UDP
} Modes;

/**
 * @brief Structure containing all the command-line arguments given to the program.
 * Makes the portability of arguments through source files easier.
 */
typedef struct arguments
{
    char* host;
    int port;
    Modes mode;
} Arguments;

/**
 * @brief Checks whether conversion of `*stringPtr` to integer was successful.
 * 
 * According to `man strtol`, _If  endptr  is not NULL, strtol() stores the address of the first invalid character in *endptr._
 * 
 * If `*endPtr` is ```'\0'```, then it points at the very end of the string - the terminating byte. 
 * That means the conversion was successful.
 * 
 * If `*stringPtr` contains only ```'\0'```, then `*endPtr` contains ```'\0'``` too but the conversion should fail. 
 * When these two are equal and both ```'\0'```, the function returns true.
 * 
 * @param stringPtr Original string converted by `strtol()`.
 * @param endPtr Address of the first invalid character.
 * @return true When conversion encountered non-numeric characters.
 * @return false When conversion was successful.
 */
bool strtolFailed(char *stringPtr, char *endPtr);

/**
 * @brief Processes string given as argument to this function.
 * If the string contains values `tcp` or `udp` in either lowercase or uppercase, appropriatemode is returned.
 * 
 * @param mode Command-line argument present after `-m` option as string.
 * @return Appropriate mode.
 */
Modes getMode(char *mode);

/**
 * @brief Checks if `portNumber` is between 0 and 65535, which is the largest existing port number.
 * 
 * @param portNumber Supposed port number.
 * @return true When `portNumber` is valid.
 * @return false When `portNumber` is out of range.
 */
bool isValidPort(int portNumber);

/**
 * @brief Converts `port` to integer and checks if it's within correct interval (see `isValidPort()`).
 * 
 * @param port Port number in string form.
 * @return Port Number as integer.
 */
int getPort(char* port);

/**
 * @brief Determines if the `arg` passed to the function had already been used.
 * 
 * Works through static array of characters representing used arguments. Loops through the array and compares used 
 * arguments with `arg`. 
 * 
 * If it's not found, then it goes through the array again until it finds a free space to be placed. Then returns false.
 * 
 * @return true When the argument has been already used.
 * @return false When the argument hasn't been used yet.
 */
bool isUserArg();

/**
 * @brief Fills up `args` with proper values. Calls functions for checking the validity and filling the structure up
 * based on the given `arg`.
 * 
 * See `getMode()`, `getPort()` for validation of arguments.
 * 
 * @param arg Currently parsed argument (e.g. `-m`, `-p` or `-h`).
 * @param argv Program command-line arguments.
 * @param args `Arguments` structure to be filled with values.
 * @param i Iterator from `processArgument()` used for indexing `**argv`. The functions called from this one require
 * value of the given command-line option.
 */
void processCurrentArgument(char arg, char **argv, Arguments *args, int *i);

/**
 * @brief Goes through all the command-line arguments given to the program and checks their validity.
 *
 * The number of arguments can be either 2 when `--help` option is present or 7 when `--help` option is not present
 * (the program expects that the user knows what type of arguments is it expecting).
 *
 * If the option is valid (e.g. it's `-m`, `-h` or `-p`), then `processCurrentArgument()` is called and passes the
 * iterator over arguments as parameter, so the `argv[iterator]` points to value expected after the option. The value
 * is then parsed too.
 *
 * @param argc Number of command-line arguments given to the program.
 * @param argv Array of command-line arguments.
 * @param args Empty structure containing space for values of command-line arguments.
 */
void processArguments(int argc, char **argv, Arguments *args);

#endif /* argument_processing_h */
