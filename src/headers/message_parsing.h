#ifndef message_parsing_h
#define message_parsing_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "argument_processing.h"
#include "error_functions.h"
#include "errors.h"
#include "constants.h"
#include "stack.h"


/**
 * @brief Valid token types.
 */
typedef enum tokenType
{
    leftPar, rightPar, operatorToken, number, newLine
} tokenType;

/**
 * @brief Contains type and data of token.
 */
typedef struct token
{
    tokenType type;
    char *data;
} Token;

/**
 * @brief Does lexical analysis of expression. Exits when invalid character is encountered.
 * 
 * @param buffer Buffer with message from client.
 * @return 0 on success, -1, on failure
 */
int lexicalAnalysis(const char *buffer);

/**
 * @brief Checks if char `c` is valid operator.
 */
bool isValidOperator(char c);

/**
 * @brief Checks if char `c` is digit, e.g. between '0' and '9'.
 */
bool isDigit(char c);

/**
 * @brief Checks if char `c` is left parenthesis.
 */
bool isLeftPar(char c);

/**
 * @brief Checks if char `c` is right parenthesis.
 */
bool isRightPar(char c);

/**
 * @brief Checks if char `c` is either space or tab. The server is benevolent
 * for tabs in expressions too.
 */
bool isWhiteSpace(char c);

/**
 * @brief Takes `op1` and `op2` and performs operation given by `operator`.
 * Saves the result as string to `*result`.
 */
void doOperation(int op1, int op2, char oper, char *result);

/**
 * @brief Evaluates prefix expression with help of one additional stack.
 * Goes through the `exp` stack, pushes numbers into new stack. When it encounters
 * operator, pops two numbers from new stack, calculates given operation and pushes 
 * the result back on the new stack. After going through the `exp` stack, result is
 * on top of new stack. It's then copied to `*result`.
 * 
 * @param exp 
 * @param result 
 */
void evaluateExpression(Stack *exp, char *result);

/**
 * @brief Fills first three bytes of `*buffer` with *opcode*, *status code* and *payload length*.
 * The rest is filled with `errorMessage`.
 */
void getErrorMessageUDP(char *buffer, char *errorMessage);

/**
 * @brief Copies `*errorMessage` to `*buffer`.
 */
void getErrorMessageTCP(char *buffer, char *errorMessage);

/**
 * @brief Fills first three bytes of `*buffer` with  *opcode*, *status code* and *payload length*.
 * The rest is filled with `*result`.
 */
void getResponseMessageUDP(char *buffer, char *result);

/**
 * @brief Fills `*buffer` with *RESULT* followed by `*result`.
 */
void getResponseMessageTCP(char *buffer, char *result);


/**
 * @brief Goes through expression char by char while skipping whitespaces.
 * 
 * Functions remembers it's position in buffer using *static* variable `position`.
 * 
 * @param buffer Buffer with expression from client.
 * @param token Address of token structure, where it will be overwritten.
 * @param newQuery Flag marking if `position` should be reset or not.
 */
void getNextToken(char *buffer, Token *token, bool newQuery);

/**
 * @brief Determines if the next token is operator (terminal).
 * 
 * @return 0 on success, -1 on fail.
 */
int parseOperator(Token *token, Stack *stack);

/**
 * @brief Parses expression non-terminal according to [protocol](https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md) grammar.
 * 
 * @param buffer Buffer with query.
 * @param token Currently processed token.
 * @param stack Stack with already processed part of query.
 * @return 0 on succes, -1 on fail.
 */
int parseExpression(char *buffer, Token *token, Stack *stack);

/**
 * @brief Parses query non-terminal according to [protocol](https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md) grammar.
 * 
 * @param buffer Buffer with query.
 * @param token Currently processed token.
 * @param stack Stack with already processed part of query.
 * @return 0 on success, -1 on fail.
 */
int parseQuery(char *buffer, Token *token, Stack *stack);

/**
 * @brief Parses query recieved from client.
 * 
 * @param buffer Query location.
 * @return 0 if the message is well-formed according to [protocol](https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md) grammar; -1 otherwise.
 */
char* parseRecievedQuery(char *buffer, void (*getResponseMessage)(char*, char*), Modes mode);

#endif /* message_parsing_h */
