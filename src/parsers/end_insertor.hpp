/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 15:41
 * @ Modified time: 2024-05-02 15:09
 */

#pragma once

#include "token.hpp"

class EndInsertor
{
private:
    /**
     * @brief Skips tokens representing function call in input tape.
     */
    static void skipFunctionCall(InputTape::iterator& tokenIter) ;

    /**
     * @brief Skips tokens representing expression operand in input tape.
     */
    static void skipOperand(InputTape::iterator& tokenIter) ;

    /**
     * @brief Skips tokens representing expression operand in parentheses in input tape.
     */
    static void skipOperandInParentheses(InputTape::iterator& tokenIter) ;

    /**
     * @brief Determines if the token is an operator
      by looking at its type.
     *
     * @param token Token to be checked.
     */
    static bool isOperator(Token& token) ;

public:
    /**
     * @brief Goes through the input tape and inserts auxiliary token at the end of currently parsed expression.
     */
    static void InsertExpressionEnd() ;

    /**
     * @brief Goes through the input tape and inserts auxiliary token at the end of currently parsed function call.
     */
    static void InsertFunctionEnd() ;
};
