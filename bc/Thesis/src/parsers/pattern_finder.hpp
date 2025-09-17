/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 16:10
 * @ Modified time: 2024-05-02 19:35
 */

#pragma once

#include "grammar.hpp"
#include "predictive_parser.hpp"
#include "token.hpp"

/**
 * @brief Auxiliary class used for finding anything on the automaton pushdown.
 */
class PatternFinder
{
private:
    /**
     * @brief The automaton pushdown.
     */
    AnalysisStack& stack;

public:
    /**
     * @brief Pattern finder constructor.
     * 
     * @param stack The automaton pushdown.
     */
    explicit PatternFinder(AnalysisStack& stack);

    /**
     * @brief Searches for topmost token on the automaton pushdown.
     *
     * @return Pointer to the topmost token.
     */
    Token* FindFirstToken();

    /**
     * @brief Searches for a handle which ends with precedence symbol '<' on the automaton pushdown.
     * 
     * @param emptyRule An empty rule to be filled with the found handle.
     */
    void FindFirstRule(Rule& emptyRule);
};