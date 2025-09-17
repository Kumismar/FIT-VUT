#ifndef list_h
#define list_h

#include "error_functions.h"
#include "errors.h"
#include <stdlib.h>

typedef struct node 
{
    void *ptr;
    struct node *next; 
} listNode;

typedef struct list
{
    listNode *first;
    listNode *current;
} ptrList;

extern ptrList *allocMemList;

/**
 * @brief Initializes the global list of pointers. 
 * 
 * The main purpose of this data structure is an unexpected exit scenario, 
 * where it'scomplicated to manually free all the memory allocated to the heap.
 * 
 * The list slows program down a little bit but this program is small so it's impossible
 * to tell the difference. The advantages outbalance all the disadvantages.
 * 
 * Prevents memory leaks.
 */
void initList();

/**
 * @brief Allocates memory of given @param size. 
 * Also adds the created pointer to the global list of pointers.
 * 
 * @return Pointer to the allocated memory.
 */
void* mallocAndAddToList(size_t size);

/**
 * @brief Goes through the list of pointers allocated on the heap 
 * and searches for @param ptr. If it's found, then the pointer is freed 
 * and removed from the list. If not, the program exits with code 99
 * which means internal error of the program.
 */
void freeAndRemoveFromList(void *ptr);

/**
 * @brief Goes through the list of allocated pointers and frees all of them.
 */
void disposeList();

#endif /* list_h */