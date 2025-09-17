#include "headers/common.h"
#include "headers/result_array.h"

CalcData lastResults[PREVIOUS_RESULTS_SIZE];
int newestResult = 0;

void InitResultArray()
{
    for (int i = 0; i < PREVIOUS_RESULTS_SIZE; i++) {
        lastResults[i].faultType = NONE;
        lastResults[i].type = INT;
        lastResults->value.intResult = 0;
    }
}

void InsertResult(CalcData* result) 
{
    static int i = PREVIOUS_RESULTS_SIZE - 1;
    newestResult = i;
    lastResults[i] = *result;
    i--;
    if (i == 0) {
        i = PREVIOUS_RESULTS_SIZE;
    }
    return;
}