#ifndef result_array_h
#define result_array_h

#include "common.h"

#define PREVIOUS_RESULTS_SIZE 5

/**
 * @brief Array of previous results from calculator. 
 * Any previous result can be picked as next operand. 
 */
extern CalcData lastResults[PREVIOUS_RESULTS_SIZE];

/**
 * @brief Index of the most recently added result to lastResults.
 */
extern int newestResult;

/**
 * @brief Initializes previous result array to all zeros.
 */
void InitResultArray();

/**
 * @brief Inserts new result from operation to lastResults array.
 * If there was a result already on index, it's overwritten.
 * 
 * @param result New result to be inserted.
 */
void InsertResult(CalcData* result);

#endif
