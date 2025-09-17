#include "headers/stack.h"

void initStack(Stack *new)
{
    new->top = NULL;
}

void pop(Stack *stack)
{
    if (stack == NULL)
        errorExit("Stack pop null", PROGRAM_FAILURE);
    else if (stack->top == NULL)
        return; 

    stackNode *tmp = stack->top;
    stack->top = stack->top->next;
    freeAndRemoveFromList(tmp);
}

char* top(Stack *stack)
{
    if (stack == NULL)
        errorExit("stack null top", PROGRAM_FAILURE);
    if (stack->top == NULL)
        return '\0';
    
    return stack->top->s;
}

void push(Stack *stack, char *s)
{
    if (stack == NULL)
        errorExit("stack push null\n", PROGRAM_FAILURE);

    stackNode *new = (stackNode*)mallocAndAddToList(sizeof(stackNode));
    new->s = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);
    strcpy(new->s, s);

    if (stack->top != NULL) 
        new->next = stack->top;
    
    stack->top = new;
}

void disposeStack(Stack *stack)
{
    if (stack == NULL)
        errorExit("Dispose stack null\n", PROGRAM_FAILURE);

    while (stack->top != NULL)
        pop(stack);
}

