#ifndef argument_processing_h
#define argument_processing_h

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "errors.h"
#include "error_functions.h"
#include "list.h"

typedef enum modes 
{
    TCP, UDP
} Modes;

/**
 * @brief Structure containing all the command-line arguments given to the program.
 * It makes implementation of argument processing a lot easier.
 */
typedef struct arguments
{
    char* host;
    int port;
    Modes mode;
} Arguments;

/**
 * @brief Determines whether conversion of string to long int was successful. 
 * If there weren't any digits at all, the endPtr and stringPtr point to the same character.
 * When invalid character is found, its address is stored in *endPtr.
 * 
 * @param stringPtr Pointer to the input strtol string.
 * @param endPtr Pointer to a string where any excess characters were writen.
 * @return True when converting string was successful.
 * @return False when converting string failed.
 */
bool strtolFailed(char *stringPtr, char *endPtr);


/**
 * @brief Checks if mode is correctly given to the program and loads it to the Arguments struct.
 * 
 * @param mode Mode as string given from user.
 * @return Mode as enum.
 */
Modes getMode(char* mode);


/**
 * @brief Checks whether one @param byte of IP Address is valid (e.g. 0-255).
 * 
 * @return True or false depending on validity of @param byte. 
 */
bool isValidByte(long byte);

/**
 * @brief Parses string @param IPAddr into sections divided by '.' and checks validity of each
 * of those parsed sections. Every section has to contain only numbers and the numbers should be
 * between 0 and 255.
 * 
 * @return The original string IP Address if valid. If not, the program exits.
 */
char* parseIPAddress(char* IPAddr);

/**
 * @brief Checks if @param portNumber is between 0 and uint16_max.
 */
bool isValidPort(int portNumber);

/**
 * @brief Converts @param port from string form to integer form.
 * 
 * @return Port in integer form.
 */
int getPort(char* port);

/**
 * @brief To prevent nesting, this function is called. It processes the current parsed argument (e.g. -m).
 * 
 * @param arg Currently processed argument. It's given to the function without '-' symbol.
 * @param argv Command-line arguments as passed to the main function.
 * @param args A structure holding all the values of arguments if they're passed to the program correctly.
 * @param i Iterator from processArguments() function.
 */
void processCurrentArgument(char arg, char **argv, Arguments *args, int *i);

/**
 * @brief Checks if the cmdl @param arg was already used.
 * Uses static array of strings and every unique argument gets loaded to the array.
 * When argument is already in the array, user typed it two times.
 * 
 * @return True when the @param arg was already used. 
 * @return False when the @param arg wasn't used yet.
 */
bool isUsedArg(char *arg);

/**
 * @brief Takes care of processing command-line arguments.
 * 
 * @param argc Number of arguments as passed to the main function.
 * @param argv Arguments themselves as passed to the main function.
 * @param args A structure holding all the values of arguments if they're passed to the program correctly.
 */
void processArguments(int argc, char **argv, Arguments *args);

#endif /* argument_processing_h */