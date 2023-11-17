#include "headers/AllocList.hpp"

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