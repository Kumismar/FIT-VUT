/**
 * @file alloc_list.hpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#pragma once

#include <list>
#include "list_insertable.hpp"

/**
 * @brief List of allocated objects of (sub)classes of ListInsertable.  
 */
extern std::list<ListInsertable*> AllocList;

/**
 * @brief Goes through the AllocList and removes all objects.
 */
void deleteAll();