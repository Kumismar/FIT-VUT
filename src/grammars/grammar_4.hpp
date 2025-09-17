/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:33
 */

#pragma once

#include "grammar.hpp"
#include <vector>

/**
 * @brief Fourth component of Koubp CD grammar system.
 */
class Grammar4 : public Grammar
{
private:
    /**
     * @brief A list of right sides of rules that can be used to expand left sides of rules.
     */
    static const std::vector<Rule> rightSideRules;

public:
    /**
     * @brief A virtual destructor for Grammar4 overriding Grammar destructor.
     */
    ~Grammar4() override = default;

    /**
     * @brief Expands left side of the rule to right side of the rule.
     * 
     * @param ruleNumber Index of rule to be expanded.
     * @return Sequence of symbols that are right side of the rule.
     */
    Rule Expand(unsigned ruleNumber) override;

    /**
     * @brief Checks if sequence of symbols is a right side of any rule.
     * 
     * @param stackRule Rule to be checked.
     * @return True if the rule is a part of the grammar, false otherwise.
     */
    static bool IsRule(Rule& stackRule);

    /**
     * @brief Cleans up all the memory allocated by the grammar.
     */
    static void Cleanup();
};