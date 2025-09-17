/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 21:56
 * @ Modified time: 2024-05-02 15:01
 */

#pragma once

#include "predictive_parser.hpp"
#include "table_index.hpp"

/**
 * @brief A class performing deep pushdown automaton operations for predictive parser.
 */
class SymbolHandler
{
private:
    /**
     * @brief The automaton stack.
     */
    AnalysisStack& stack;

    /**
     * @brief Right side of rule expanded by one of the components.
     */
    Rule expandedRightSide;

    /**
     * @brief Pushes a rule to the automaton pushdown.
     * 
     * @param ruleLhs Left hand side of rule used for expansion.
     */
    void pushRule(Nonterminal* ruleLhs);

    /**
     * @brief Checks if left hand side of the rule was expanded to epsilon.
     * 
     * @return True if epsilon was expanded, false otherwise.
     */
    bool returnedEpsilon();

public:
    /**
     * @brief Symbol handler constructor.
     * 
     * @param stack The automaton stack.
     */
    explicit SymbolHandler(AnalysisStack& stack);

    /**
     * @brief Expands the topmost nonterminal on automaton stack based on the LL table entry.
     * 
     * @param parsingFunction A flag indicating the parser are currently parsing a function call.
     * @param tableItem The LL table entry with grammar number and rule number.
     */
    void Expand(bool parsingFunction, LLTableIndex& tableItem);

    /**
     * @brief Pops the topmost token from the automaton stack.
     */
    void Pop();
};