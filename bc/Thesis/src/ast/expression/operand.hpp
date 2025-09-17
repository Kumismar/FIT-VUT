/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:23
 * @ Modified time: 2024-05-01 12:38
 */

#pragma once

#include "expression.hpp"
#include "token.hpp"

/**
 * @brief A class representing an operand in the AST.
 */
class Operand : public Expression
{
protected:
    /**
     * @brief Data of the operand taken from token.
     */
    TokenData data;
};