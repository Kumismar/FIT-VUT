#ifndef list_h
#define list_h

#include "error_functions.h"
#include "errors.h"
#include <stdlib.h>

/**
 * @brief A simple node containing pointer to the allocated memory and pointer to the next node.
 */
typedef struct listnode
{
    void *ptr;
    struct listnode *next;
} listNode;

/**
 * @brief Linked list of pointers to the allocated memory on heap.
 * 
 * The main purpose of this data structure is an unexpected exit scenario, 
 * where it's complicated to manually free all the memory allocated on the heap.
 * This data structure makes it easy to free the allocated memory and prevent memory leaks.
 * 
 * However, since it's a linear data structure, the time complexity O(n) is not optimal. It can be optimized 
 * with using different data structures, such as AVL trees or hashtables, but this is a small program and
 * the performance issue is not noticable.
 */
typedef struct list
{
    listNode *first;
    listNode *current;
} ptrList;

extern ptrList *allocMemList;

/**
 * @brief Initializes the global list of pointers.
 */
void initList();

/**
 * @brief Allocates memory of given size.
 * Also adds the created pointer to the global list of pointers.
 *
 * @param size Size in bytes to be allocated.
 * @return Pointer to the allocated memory.
 */
void* mallocAndAddToList(size_t size);

/**
 * @brief Goes through the list of pointers allocated on the heap 
 * and searches for pointer given as argument.
 * If it's found, then the pointer is freed and removed from the list.
 * If not, the program exits with code 99 which means internal error of the program.
 * 
 * @param ptr Pointer to be freed.
 */
void freeAndRemoveFromList(void *ptr);

/**
 * @brief Goes through the list of pointers and frees all the memory on their addresses.
 */
void disposeList();

#endif /* list_h */