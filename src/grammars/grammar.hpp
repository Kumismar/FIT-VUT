/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:34
 */

#pragma once

#include "stack_item.hpp"
#include <list>

typedef std::list<Symbol*> Rule;

/**
 * @brief Abstract class representing a grammar.
 */
class Grammar
{
public:
    /**
     * @brief A virtual destructor for Grammar.
     */
    virtual ~Grammar() = default;

    /**
     * @brief Expands left side of the rule to right side of the rule.
     * @details Every grammar has the same implementation but works with different rules.
     *
     * @param ruleNumber Index of rule to be expanded.
     * @return Sequence of symbols that are right side of the rule.
     */
    virtual Rule Expand(unsigned ruleNumber) = 0;
};
