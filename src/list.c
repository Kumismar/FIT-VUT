#include "../headers/list.h"

ptrList list;
ptrList *allocMemList = NULL;

void initList()
{
    allocMemList = &list;
    allocMemList->first = NULL;
    allocMemList->current = NULL;
}

void* mallocAndAddToList(size_t size)
{
    if (allocMemList == NULL)
        errorExit("Uninitialized list when trying to malloc.\n", PROGRAM_FAILURE);

    void *ptr = malloc(size);
    listNode *tmp = (listNode*)malloc(sizeof(listNode));

    if (allocMemList->first == NULL)
    {
        allocMemList->first = tmp;
        allocMemList->current = tmp;
    }
    else
    {
        allocMemList->current->next = tmp;
        tmp->next = NULL;
        allocMemList->current = tmp;
    }
    allocMemList->current->ptr = ptr;

    return ptr; 
}

void freeAndRemoveFromList(void *ptr)
{
    if (ptr == NULL)
        return;
    else if (allocMemList == NULL)
        errorExit("Uninitialized list when trying to free.\n", PROGRAM_FAILURE);
    
    listNode *tmp = allocMemList->first;

    if (tmp->ptr == ptr)
    {
        allocMemList->first = tmp->next;
        free(tmp->ptr);
        free(tmp);
        return;
    }

    listNode *previous = tmp;
    while (tmp != NULL)
    {
        if (tmp->ptr == ptr)
        {
            previous->next = tmp->next;
            free(tmp->ptr);
            free(tmp);
            return;
        }
        previous = tmp;
        tmp = tmp->next;
    }

    errorExit("Pointer to free not found in list.\n", PROGRAM_FAILURE);
}

void disposeList()
{
    if (allocMemList == NULL)
        errorExit("List not initialized when disposing.\n", PROGRAM_FAILURE);
    else if (allocMemList->first == NULL)
        return;

    listNode *tmp = allocMemList->first;
    listNode *nodeToFree;
    while (tmp != NULL)
    {
        nodeToFree = tmp;
        tmp = tmp->next;

        if (nodeToFree->ptr != NULL)
            free(nodeToFree->ptr);
        
        free(nodeToFree);
    }
}

int listLength()
{
    if (allocMemList == NULL)
        errorExit("Unitialized list when calculating list length.\n", PROGRAM_FAILURE);

    int count = 0;
    for (listNode *tmp = allocMemList->first; tmp != NULL; tmp = tmp->next)
        count++;

    return count;
}

