#ifndef calculator_h
#define calculator_h

#include "common.h"

#define OP_MIN 0
#define OP_MAX 5

/**
 * @brief Enumerator of program states; not every operator requires two operands.
 */
typedef enum calculatorStates {
    OPERAND1, OPERATOR, OPERAND2, CALCULATE
} CalcState;

/**
 * @brief Enumerator of calculator operators available for user.
 */
typedef enum operators {
    ADD, SUB, MUL, DIV, SQRT, POW
} Operator;

/**
 * @brief Cycle through lastResults array and select operand on encoder button click.
 * 
 * @param operandNumber Operand number needed for correct printing to user.
 * @return Operand chosen.
 */
CalcData PickFromPreviousResults(int operandNumber);

/**
 * @brief Display operand via UART5 to user.
 * 
 * @param operand Operand to display.
 * @param operandString String with operand values stored.
 */
void DisplayOperand(CalcData operand, char* operandString);

/**
 * @brief Cycle through integers or choose operand from last result with encoders.
 * Actively wait for user interaction with board.
 * 
 * @param operandNumber Operand number needed for correct printing to user.
 * @return Operand chosen.
 */
CalcData PickNumber(int operandNumber);

/**
 * @brief Maps Operator enum to string.
 * 
 * @param operator 
 * @return Operator as string for displaying to user.
 */
char* mapOperator(Operator operator);

/**
 * @brief Display operator via UART5 to user.
 * 
 * @param operator Operator converted to int.
 * @param operatorString Operator to be displayed
 */
void DisplayOperator(int operator, char* operatorString);

/**
 * @brief Cycle through available operators and choose one with rotary encoder.
 * Actively waits for user interaction with board.
 * 
 * @return Picked operator.
 */
Operator PickOperator();

/**
 * @brief Call computing function from matlib.c based on operator given.
 * 
 * @param operand1 
 * @param operand2 
 * @return Result of the operation.
 */
CalcData CalculateResult(CalcData operand1, Operator operator, CalcData operand2);

/**
 * @brief Get error message based on fault given. 
 * The message will be then printed to user.
 * 
 * @param fault 
 * @return Error message.
 */
char* GetErrorMessage(FaultType fault);

/**
 * @brief Send result operation with UART5 to user.
 * 
 * @param result Operation result.
 */
void DisplayResult(CalcData* result);

#endif