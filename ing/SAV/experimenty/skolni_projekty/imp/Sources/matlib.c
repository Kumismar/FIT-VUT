#include "headers/matlib.h"
#include "headers/common.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

int isNearZero(float num)
{
    float epsilon = 1e-10;
    if (0 < num && num < epsilon ||
        -epsilon < num && num < 0) {
        return TRUE;
    }
    return FALSE;
}

void Add(CalcData* result, CalcData operand1, CalcData operand2)
{
    result->type = (operand1.type == INT && operand2.type == INT) ? INT : FLOAT;
    if (operand1.type == INT && operand2.type == INT) {
        result->value.intResult = operand1.value.intResult + operand2.value.intResult;
    }
    else if (operand1.type == INT && operand2.type == FLOAT) {
        result->value.floatResult = operand1.value.intResult + operand2.value.floatResult;
    }
    else if (operand1.type == FLOAT && operand2.type == INT) {
        result->value.floatResult = operand1.value.floatResult + operand2.value.intResult;
    }
    else {
        result->value.floatResult = operand1.value.floatResult + operand2.value.floatResult;
    }
}

void Sub(CalcData* result, CalcData operand1, CalcData operand2)
{
    result->type = (operand1.type == INT && operand2.type == INT) ? INT : FLOAT;
    if (operand1.type == INT && operand2.type == INT) {
        result->value.intResult = operand1.value.intResult - operand2.value.intResult;
    }
    else if (operand1.type == INT && operand2.type == FLOAT) {
        result->value.floatResult = operand1.value.intResult - operand2.value.floatResult;
    }
    else if (operand1.type == FLOAT && operand2.type == INT) {
        result->value.floatResult = operand1.value.floatResult - operand2.value.intResult;
    }
    else {
        result->value.floatResult = operand1.value.floatResult - operand2.value.floatResult;
    }
}

void Mul(CalcData* result, CalcData operand1, CalcData operand2)
{
    result->type = (operand1.type == INT && operand2.type == INT) ? INT : FLOAT;
    if (operand1.type == INT && operand2.type == INT) {
        result->value.intResult = operand1.value.intResult * operand2.value.intResult;
    }
    else if (operand1.type == INT && operand2.type == FLOAT) {
        result->value.floatResult = operand1.value.intResult * operand2.value.floatResult;
    }
    else if (operand1.type == FLOAT && operand2.type == INT) {
        result->value.floatResult = operand1.value.floatResult * operand2.value.intResult;
    }
    else {
        result->value.floatResult = operand1.value.floatResult * operand2.value.floatResult;
    }
}

void Div(CalcData* result, CalcData operand1, CalcData operand2)
{
    result->type = FLOAT;
    if (operand2.type == INT) {
        if (operand2.value.intResult == 0) {
            result->type = FAULT;
            result->faultType = DIVISION_ZERO;
            return;
        }

        if (operand1.type == INT) {
            result->value.floatResult = (float)operand1.value.intResult / (float)operand2.value.intResult;
        }
        else {
            result->value.floatResult = operand1.value.floatResult / (float)operand2.value.intResult;
        }
    }
    else { // operand2 is float
        if (isNearZero(operand2.value.floatResult)) {
            result->type = FAULT;
            result->faultType = DIVISION_ZERO;
            return;
        }

        if (operand1.type == INT) {
            result->value.floatResult = (float)operand1.value.intResult / operand2.value.floatResult; 
        }
        else { // operand1 is float
            result->value.floatResult = operand1.value.floatResult / operand2.value.floatResult; 
        }
    }
}

void CalculateSqrt(CalcData* result, CalcData operand1) 
{  
    result->type = FLOAT;
    if (operand1.type == INT) {
        if (operand1.value.intResult < 0) {
            result->type = FAULT;
            result->faultType = SQRT_LT_ZERO;
            return;
        }
        result->value.floatResult = sqrt(operand1.value.intResult);
    }
    else { // operand1 is float
        if (operand1.value.floatResult < 0) {
            result->type = FAULT;
            result->faultType = SQRT_LT_ZERO;
            return;
        }
        result->value.floatResult = sqrt(operand1.value.floatResult);
    }
}

void CalculatePow(CalcData* result, CalcData operand1, CalcData operand2)
{
    result->type = FLOAT;
    if (operand1.type == INT && operand2.type == INT) {
        result->value.floatResult = pow(operand1.value.intResult, operand2.value.intResult);
    }
    else if (operand1.type == FLOAT && operand2.type == INT) {
        result->value.floatResult = pow(operand1.value.floatResult, operand2.value.intResult);
    }
    else if (operand1.type == INT && operand2.type == FLOAT) {
        result->value.floatResult = pow(operand1.value.intResult, operand2.value.floatResult);
    }
    else {
        result->value.floatResult = pow(operand1.value.floatResult, operand2.value.floatResult);
    }

    if (isnan(result->value.floatResult)) {
        result->type = FAULT;
        result->faultType = NAN_FAULT;
    }
    else if (isinf(result->value.floatResult)) {
        result->type = FAULT;
        result->faultType = INF_FAULT;
    }
}   
