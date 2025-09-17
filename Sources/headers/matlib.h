#ifndef matlib_h
#define matlib_h

#include "headers/common.h"

/**
 * @brief Checks if floating-point number is around zero.
 * For this specific function, anything between -1e-10 and 1e-10 is considered zero.
 * For this program, extreme precision isn't needed. 
 * 
 * @param num Number to be checked. 
 * @return 1 if true, 0 if false.
 */
int isNearZero(float num);

/**
 * @brief Add two numbers and save it to result.
 * Check what data types are operands and add matching values.
 * 
 * @param result 
 * @param operand1 
 * @param operand2 
 */
void Add(CalcData* result, CalcData operand1, CalcData operand2);

/**
 * @brief Subtract two numbers and save it to result.
 * Check what data types are operands and subtract matching values.
 * 
 * @param result 
 * @param operand1 
 * @param operand2 
 */
void Sub(CalcData* result, CalcData operand1, CalcData operand2);

/**
 * @brief Multiply two numbers and save it to result.
 * Check what data types are operands and multiply matching values.
 * 
 * @param result 
 * @param operand1 
 * @param operand2 
 */
void Mul(CalcData* result, CalcData operand1, CalcData operand2);

/**
 * @brief Divide two numbers and save it to result.
 * Check what data types are operands and divide matching values.
 * Sets faultType to DIVISION_ZERO when dividing by zero.
 * 
 * @param result 
 * @param operand1 
 * @param operand2 
 */
void Div(CalcData* result, CalcData operand1, CalcData operand2);

/**
 * @brief Compute square root of operand1 and save it to result.
 * Sets faultType to SQRT_LT_ZERO if operand < 0.
 * 
 * @param result 
 * @param operand1 
 */
void CalculateSqrt(CalcData* result, CalcData operand1);

/**
 * @brief Compute operand1 to the power of operand2.
 * Sets faultType if result is +-NaN or +-infinity.
 * 
 * @param result 
 * @param operand1 
 * @param operand2 
 */
void CalculatePow(CalcData* result, CalcData operand1, CalcData operand2);

#endif