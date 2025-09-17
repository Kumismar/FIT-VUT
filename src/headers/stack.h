#ifndef stack_h
#define stack_h

#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "error_functions.h" 
#include "constants.h"

/**
 * @brief Stack node containing data in `*s`.
 */
typedef struct stacknode
{
    char *s;
    struct stacknode *next;
} stackNode;

/**
 * @brief Implementation of stack using singly linked list.
 */
typedef struct stack
{
    stackNode *top;
} Stack;

/**
 * @brief Initializes stack.
 * 
 * Note that it doesn't declare it for you; declare it first and then pass the `new`
 * stack to this function.
 */
void initStack(Stack *newStack);

/**
 * @brief Returns pointer to data on top of `*stack`.
 */
char* top(Stack *stack);

/**
 * @brief Removes node on top of `*stack` and moves stack->top pointer.  
 */
void pop(Stack *stack);

/**
 * @brief Inserts new node with data `*s` on top of `*stack`.
 */
void push(Stack *stack, char *s);

/**
 * @brief Deletes all `*stack` nodes. The `*stack` itself remains because it's statically allocated. 
 */
void disposeStack(Stack *stack);

#endif /* stack_h */