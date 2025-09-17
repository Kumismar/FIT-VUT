/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 21:19
 * @ Modified time: 2024-05-02 15:15
 */

#pragma once

#include "predictive_parser.hpp"

/**
 * @brief A class performing pushdown automaton operations for precedence parser.
 */
class ExpressionProcessor
{
private:
    /**
     * @brief The automaton stack.
     */
    AnalysisStack& stack;

    /**
     * @brief Pushes a precedence symbol '<' to the automaton pushdown.
     * @details The position of precedence symbol is based on pushdown top symbol.
     * If the top symbol is a nonterminal, the symbol is pushed after it, otherwise right on top.
     */
    void pushPrecedence();

public:
    /**
     * @brief Expression processor constructor.
     * 
     * @param stack The automaton stack.
     */
    explicit ExpressionProcessor(AnalysisStack& stack);

    /**
     * @brief Reduces the expression on the automaton stack if there is a rule to reduce.
     */
    void Reduce();

    /**
     * @brief Shifts the '<' symbol and input token onto the automaton stack.
     */
    void Shift();

    /**
     * @brief Pushes the input token onto the automaton stack.
     */
    void Push();
};