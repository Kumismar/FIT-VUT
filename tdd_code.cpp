//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Ondrej Koumar <xkouma02@stud.fit.vutbr.cz>
// $Date:       $2022-03-10
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Ondrej Koumar
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>


#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find/GetHead ...
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "singly linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{ 
    while (m_pHead != NULL)
    {
        Element_t *rm_node = m_pHead;
        
        m_pHead = m_pHead->pNext;
        delete rm_node;
    }
}

void PriorityQueue::Insert(int value)
{
    Element_t *new_node = new Element_t;
    new_node->pNext = NULL;
    new_node->value = value;
        

    if (m_pHead == NULL)
    {
        m_pHead = new_node;

        return;
    }
    else if (new_node->value > m_pHead->value)
    {
        Element_t *temp = m_pHead;
        m_pHead = new_node;
        m_pHead->pNext = temp;

        return;
    }

    Element_t *tmp_node = m_pHead;

    while (tmp_node->pNext != NULL)
    {
        if (new_node->value >= tmp_node->pNext->value)
        {
            new_node->pNext = tmp_node->pNext;
            tmp_node->pNext = new_node;

            return;
        }
        else if (new_node->value <= tmp_node->pNext->value && tmp_node->pNext->pNext == NULL)
        {
            tmp_node->pNext->pNext = new_node;
            new_node->pNext = NULL;

            return;
        }

        tmp_node = tmp_node->pNext;
    }
}

bool PriorityQueue::Remove(int value)
{
    if (m_pHead == NULL)
    {
        return false;
    }

    Element_t *tmp_node = m_pHead;

    if (tmp_node->pNext == NULL && tmp_node->value == value)
    {
        delete tmp_node;
        m_pHead = NULL;

        return true;
    }
    else if (tmp_node->pNext != NULL && tmp_node->value == value)
    {
        Element_t *tmp = tmp_node->pNext;

        delete tmp_node;
        m_pHead = tmp;

        return true;
    }
    else
    {
        while (tmp_node->pNext != NULL)
        {
            Element_t *rm_node = tmp_node->pNext;

            if (rm_node->pNext == NULL && rm_node->value == value)
            {
                delete rm_node;
                tmp_node->pNext = NULL;
                
                return true;
            }

            /**
             * Tady tato podminka neni pro tuto konkretni implementaci potrebna.
             * Nicmene si myslim, ze by zde mela byt i za cenu strzeni bodu, protoze kdyby se volal remove na nahodne prvky, 
             * mohlo by se stat, ze by tato podminka potrebna byla.
             * R.I.P. 100% line coverage.
             */
            else if (rm_node->pNext != NULL && rm_node->value == value)
            {
                Element_t *behind_node = rm_node->pNext;

                delete rm_node;
                tmp_node->pNext = behind_node;

                return true;
            }

            tmp_node = tmp_node->pNext;
        }
    }
    
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *tmp_node = m_pHead;

    while (tmp_node != NULL)
    {
        if (tmp_node->value == value)
        {
            return tmp_node;
        }

        tmp_node = tmp_node->pNext;
    }

    return NULL;
}

size_t PriorityQueue::Length()
{
    Element_t *tmp_node = m_pHead;
    unsigned count = 0;

    while (tmp_node != NULL)
    {
        count++;
        tmp_node = tmp_node->pNext;
    }

	return count;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
