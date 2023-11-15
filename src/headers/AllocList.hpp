#pragma once

#include <list>
#include "ListInsertable.hpp"

/**
 * @brief List of allocated objects of (sub)classes of ListInsertable.  
 */
extern std::list<ListInsertable*> AllocList;

/**
 * @brief Goes through the AllocList and removes all objects.
 */
void deleteAll();