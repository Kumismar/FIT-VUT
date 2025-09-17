/**
 * @file list_insertable.hpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#pragma once

#include <list>

/**
 * @brief Auxiliary class from which all the other classes inherit.
 * Used only for list of object allocations (see alloc_list.hpp).
 */
class ListInsertable
{
public:
    virtual ~ListInsertable() {}
};
