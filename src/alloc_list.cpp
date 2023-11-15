#include "headers/alloc_list.hpp"

void deleteAll()
{
    for (auto it = AllocList.begin(); it != AllocList.end();)
    {
        delete *it;
        it = AllocList.erase(it);
    }
    AllocList.clear();
}