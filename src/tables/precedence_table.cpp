/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-01 18:37
 */

#include "precedence_table.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

const std::vector<std::vector<char>> PrecedenceTable::precTable = {
    // +    -     *    /     .    &&   ||    ==   !=     >    <   >=   <=     =     (    )     !    $    i   un-
    { '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // +
    { '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // -
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // *
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // /
    { '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // .
    { '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>', '<', '<' }, // &&
    { '<', '<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>', '<', '<' }, // ||
    { '<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // ==
    { '<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // !=
    { '<', '<', '<', '<', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // >
    { '<', '<', '<', '<', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // <
    { '<', '<', '<', '<', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // >=
    { '<', '<', '<', '<', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '<' }, // <=
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'x', '<', '>', '<', '>', '<', '<' }, // =
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', 'x', '<', '>' }, // (
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'x', '>', 'x', '>', 'x', '<' }, // )
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '<', 'x' }, // !
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'x', '<', 'x', '<', '<' }, // $
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'x', '>', 'x', '>', 'x', '>' }, // i
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', 'x', '>', '<', '<' } // un-
};

PrecRowAccessor::PrecRowAccessor(const std::vector<char>& r)
    : row(r)
{
}


char PrecRowAccessor::operator[](const Token& t)
{
    unsigned colIndex = PrecedenceTable::MapTokenToIndex(t);
    return this->row[colIndex];
}

PrecRowAccessor PrecedenceTable::operator[](const Token& t) const
{
    unsigned rowIndex = PrecedenceTable::MapTokenToIndex(t);
    return { PrecedenceTable::precTable[rowIndex] };
}

unsigned PrecedenceTable::MapTokenToIndex(const Token& t)
{
    switch (t.GetTokenType()) {
        case TokenType::t_Plus:
            return 0;
        case TokenType::t_Minus:
            return 1;
        case TokenType::t_Mul:
            return 2;
        case TokenType::t_Div:
            return 3;
        case TokenType::t_Concat:
            return 4;
        case TokenType::t_And:
            return 5;
        case TokenType::t_Or:
            return 6;
        case TokenType::t_Eq:
            return 7;
        case TokenType::t_NEq:
            return 8;
        case TokenType::t_Greater:
            return 9;
        case TokenType::t_Less:
            return 10;
        case TokenType::t_GEq:
            return 11;
        case TokenType::t_LEq:
            return 12;
        case TokenType::t_Assign:
            return 13;
        case TokenType::t_LPar:
            return 14;
        case TokenType::t_RPar:
            return 15;
        case TokenType::t_Excl:
            return 16;
        case TokenType::t_ExpEnd:
            return 17;
        // funcName will be changed for TokenType::t_Const on stack
        case TokenType::t_Const:
        case TokenType::t_FuncConst:
        case TokenType::t_Variable:
            return 18;
        case TokenType::t_UnMinus:
            return 19;
        default:
            throw SyntaxError("PrecedenceTable::MapTokenToIndex invalid token type");
    }
}