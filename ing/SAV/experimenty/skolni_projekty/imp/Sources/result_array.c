// #include "headers/result_array.h"

// #include "headers/common.h"

#define PREVIOUS_RESULTS_SIZE 5

typedef enum
{
    NONE,
    INT
} Type;

typedef struct
{
    Type faultType;
    Type type;

    union
    {
        int intResult;
        float floatResult;  // Pro uplnost, i kdyz to nepouzijeme
    } value;
} CalcData;

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

#ifdef CBMC_VERIFICATION

int main()
{
    InitResultArray();

    CalcData dummy;
    dummy.type = INT;
    dummy.value.intResult = 42;

    for (int k = 0; k < PREVIOUS_RESULTS_SIZE + 2; k++) {
        InsertResult(&dummy);
    }

    return 0;
}
#endif