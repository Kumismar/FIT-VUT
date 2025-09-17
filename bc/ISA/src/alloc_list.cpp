/**
 * @file alloc_list.cpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#include "headers/alloc_list.hpp"

std::list<ListInsertable*> AllocList;

void deleteAll()
{
    for (auto it = AllocList.begin(); it != AllocList.end();)
    {
        delete *it;
        it = AllocList.erase(it);
    }
    AllocList.clear();
}