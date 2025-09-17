#include "headers/fsm.h"
#include "headers/calculator.h"
#include "headers/common.h"
#include "headers/uart.h"
#include "headers/matlib.h"
#include "headers/result_array.h"

#include <stdio.h>
#include <string.h>

void FormatFloat(float floatValue, char* tmpString) 
{
    // Protoze sprintf() na teto platforme neumi format %f, je potreba extrahovat celou a desetinnou cast jako integery
    char* sign = (floatValue < 0) ? "-" : "";
    float tmpResult = (floatValue < 0) ? -floatValue : floatValue;

    int wholePart = tmpResult;
    float tmpDecimal = tmpResult - wholePart;
    int decimalPart = tmpDecimal * 1e8;
    sprintf(tmpString, "%s%d.%08d", sign, wholePart, decimalPart);
}

CalcData PickFromPreviousResults(int operandNumber)
{   
    int i = newestResult;
    CalcData previousResult = lastResults[i];
    char operandString[LEN];

    SendStr("\r\nZvolte z minulych vysledku: ");

    memcpy(operandString, 0 , LEN);
    if (previousResult.type == INT) {
        sprintf(operandString, "%d", previousResult.value.intResult);  
    }
    else if (previousResult.type == FLOAT) {
        char tmp[LEN];
        FormatFloat(previousResult.value.floatResult, tmp);
        sprintf(operandString, "%s", tmp);
    }
    SendStr(operandString);
    delay(1000000);
    int boardInteraction = ReadBoardInteraction(PICK_FROM_PREVIOUS);
    while (boardInteraction != ENCODER1_BTN_PRESSED) {
        if (boardInteraction == ENCODER_CCW) {
            beep(EN2, CCW);
            if (i == 0) {
                i = PREVIOUS_RESULTS_SIZE - 1;
            }
            else {
                i--;
            }
            previousResult = lastResults[i];
            if (previousResult.type == FAULT) {
                continue;
            }
            DisplayOperand(previousResult, operandString);
        }
        else if (boardInteraction == ENCODER_CW) {
            beep(EN2, CW);
            if (i == PREVIOUS_RESULTS_SIZE - 1) {
                i = 0;
            }
            else {
                i++;
            }
            previousResult = lastResults[i];
            if (previousResult.type == FAULT) {
                continue;
            }
            DisplayOperand(previousResult, operandString);

        }
        else if (boardInteraction == CLEAR_BTN) { 
            beep(CLR, CLR);
            i = newestResult;
            previousResult = lastResults[i];
            DisplayOperand(previousResult, operandString);
        }
        boardInteraction = ReadBoardInteraction(PICK_FROM_PREVIOUS);
    }
    return previousResult;
}

void DisplayOperand(CalcData operand, char* operandString)
{
    int stringLen = strlen(operandString);
    for (int i = 0; i < stringLen; i++) {
        SendStr("\b \b");
    }
    memcpy(operandString, 0, LEN);
    if (operand.type == INT) {
        sprintf(operandString, "%d", operand.value.intResult);
    }
    else { // FLOAT
        char tmp[LEN];
        FormatFloat(operand.value.floatResult, tmp);
        sprintf(operandString, "%s", tmp);
    }
    SendStr(operandString);
}

CalcData PickNumber(int operandNumber) 
{
    // Calculator operand to be returned
    CalcData operand = {
        .faultType = NONE,
        .type = INT,
        .value.intResult = 0
    };
    char operandString[LEN];

    // Display 0 without clearing anything first
    memcpy(operandString, 0, LEN);
    sprintf(operandString, "%d", operand.value.intResult);
    SendStr(operandString);

    int boardInteraction = ReadBoardInteraction(CHOOSE_OPERAND_OR_OPERATOR);
    while (boardInteraction != ENCODER1_BTN_PRESSED) {
        switch (boardInteraction) {
            case ENCODER2_BTN_PRESSED: {
                beep(EN2, BTN);
                operand = PickFromPreviousResults(operandNumber);
                break;
            }
            case ENCODER_CW: {
                beep(EN1, CW);
                operand.value.intResult++;
                DisplayOperand(operand, operandString);
                break;
            }
            case ENCODER_CCW: {
                beep(EN1, CCW);
                operand.value.intResult--;
                DisplayOperand(operand, operandString);
                break;
            }
            case CLEAR_BTN: {
                beep(CLR, CLR);
                operand.value.intResult = 0;
                DisplayOperand(operand, operandString);
                break;
            }
            default: {
                break;
            }
        }
        if (boardInteraction == ENCODER2_BTN_PRESSED) break;
        boardInteraction = ReadBoardInteraction(CHOOSE_OPERAND_OR_OPERATOR);
    }
    beep(EN1, BTN);
    memcpy(operandString, 0 , LEN);
    if (operand.type == INT) {
        sprintf(operandString, "\r\n\e[33mOperand %d: %d\e[0m\r\n\r\n", operandNumber, operand.value.intResult);
    }
    else if (operand.type == FLOAT) {
        char tmp[LEN];
        FormatFloat(operand.value.floatResult, tmp);
        sprintf(operandString, "\r\n\e[33mOperand %d: %s\e[0m\r\n\r\n", operandNumber, tmp);
    }
    SendStr(operandString);
    return operand;
}

char* mapOperator(Operator operator) 
{
    switch (operator) {
        case ADD: {
            return "+";
        }
        case SUB: {
            return "-";
        }
        case MUL: {
            return "*";
        }
        case DIV: {
            return "/";
        }
        case SQRT: {
            return "sqrt";
        }
        case POW: {
            return "pow";
        }
        default: {
            return "+";
        }
    }
}

void DisplayOperator(int operator, char* operatorString)
{
    int stringLen = strlen(operatorString);
    for (int i = 0; i < stringLen; i++) {
        SendStr("\b \b");
    }
    memcpy(operatorString, 0, LEN);
    sprintf(operatorString, "%s", mapOperator((Operator)operator));
    SendStr(operatorString);
}

Operator PickOperator() 
{
    // Calculator operand to be returned
    Operator operator = ADD;
    int operatorInt = 0; // auxiliary
    char operatorString[LEN];

    memcpy(operatorString, 0, LEN);
    sprintf(operatorString, "%s", mapOperator(operatorInt));
    SendStr(operatorString);

    int boardInteraction = ReadBoardInteraction(CHOOSE_OPERAND_OR_OPERATOR);
    while (boardInteraction != ENCODER1_BTN_PRESSED) {
        if (boardInteraction == ENCODER_CCW) {
            beep(EN2, CCW);
            if (operatorInt == OP_MIN) {
                operatorInt = OP_MAX;
            }
            else {
                operatorInt--;
            }
            DisplayOperator(operatorInt, operatorString);
        }
        else if (boardInteraction == ENCODER_CW) { 
            beep(EN2, CW);
            if (operatorInt == OP_MAX) {
                operatorInt = OP_MIN;
            }
            else {
                operatorInt++;
            }
            DisplayOperator(operatorInt, operatorString);
        }
        else if (boardInteraction == CLEAR_BTN) { // 
            beep(CLR, CLR);
            operatorInt = OP_MIN;
            DisplayOperator(operatorInt, operatorString);
        }
        boardInteraction = ReadBoardInteraction(CHOOSE_OPERAND_OR_OPERATOR);
    }   
    beep(EN2, BTN);
    sprintf(operatorString, "\r\n\e[33mOperator: %s\e[0m\r\n\r\n", mapOperator(operatorInt));
    SendStr(operatorString);
    return (Operator)(operatorInt);
}

CalcData CalculateResult(CalcData operand1, Operator operator, CalcData operand2) 
{
    Type operandType1 = operand1.type;
    Type operandType2 = operand2.type;

    Value operandVal1 = operand1.value;
    Value operandVal2 = operand2.value;

    CalcData result = {
        .type = INT,
        .value.intResult = 0,
        .faultType = NONE
    };

    switch (operator) {
        case ADD: {
            Add(&result, operand1, operand2);
            break;
        }
        case SUB: {
            Sub(&result, operand1, operand2);
            break;
        }
        case MUL: {
            Mul(&result, operand1, operand2);
            break;
        }
        case DIV: {
            Div(&result, operand1, operand2);
            break;
        }
        case SQRT: {
            CalculateSqrt(&result, operand1);
            break;
        }
        case POW: {
            CalculatePow(&result, operand1, operand2);
            break;
        }
        default: {
            break;
        }
    }
    InsertResult(&result);
    return result;
}

char* GetErrorMessage(FaultType fault)
{
    switch (fault)
    {
        case DIVISION_ZERO: {
            return "deleni nulou.";
        }
        case SQRT_LT_ZERO: {
            return "cislo v odmocnine < 0.";
        }
        case INF_FAULT: {
            return "vysledek je nekonecno.";
        }
        case NAN_FAULT: {
            return "vysledek je NaN.";
        }
        default: {
            return "";
        }
    }
}

void DisplayResult(CalcData* result) 
{
    char resultString[LEN] = {0};
    if (result->type == INT) {
        sprintf(resultString, "\r\n\e[32mVysledek: %d\e[0m\r\n\r\n", result->value.intResult);
    } 
    else if (result->type == FLOAT) {
    	char tmpString[LEN] = {0};
        FormatFloat(result->value.floatResult, tmpString);
        sprintf(resultString, "\r\n\e[32mVysledek: %s\e[0m\r\n\r\n", tmpString);
    } 
    else {
        char* errorMessage = GetErrorMessage(result->faultType);
        sprintf(resultString, "\r\n\e[31mChyba: %s\e[0m\r\n\r\n", errorMessage);
    } 
    SendStr(resultString);
}
