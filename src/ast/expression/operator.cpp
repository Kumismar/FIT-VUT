/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-16 11:41
 * @ Modified time: 2024-04-16 13:10
 */

#include "operator.hpp"
#include "internal_error.hpp"
#include "token.hpp"
#include <fstream>

Operator::Operator(TokenType t)
{
    switch (t) {
        case TokenType::t_Plus: {
            this->type = OperatorType::optr_Plus;
            break;
        }
        case TokenType::t_Minus: {
            this->type = OperatorType::optr_Minus;
            break;
        }
        case TokenType::t_Mul: {
            this->type = OperatorType::optr_Multiply;
            break;
        }
        case TokenType::t_Div: {
            this->type = OperatorType::optr_Divide;
            break;
        }
        case TokenType::t_Concat: {
            this->type = OperatorType::optr_Concat;
            break;
        }
        case TokenType::t_Assign: {
            this->type = OperatorType::optr_Assign;
            break;
        }
        case TokenType::t_Eq: {
            this->type = OperatorType::optr_Equal;
            break;
        }
        case TokenType::t_NEq: {
            this->type = OperatorType::optr_NotEqual;
            break;
        }
        case TokenType::t_Less: {
            this->type = OperatorType::optr_Less;
            break;
        }
        case TokenType::t_LEq: {
            this->type = OperatorType::optr_LessEqual;
            break;
        }
        case TokenType::t_Greater: {
            this->type = OperatorType::optr_Greater;
            break;
        }
        case TokenType::t_GEq: {
            this->type = OperatorType::optr_GreaterEqual;
            break;
        }
        case TokenType::t_And: {
            this->type = OperatorType::optr_And;
            break;
        }
        case TokenType::t_Or: {
            this->type = OperatorType::optr_Or;
            break;
        }
        case TokenType::t_Excl: {
            this->type = OperatorType::optr_Not;
            break;
        }
        case TokenType::t_UnMinus: {
            this->type = OperatorType::optr_UnaryMinus;
            break;
        }
        case TokenType::t_LPar: {
            this->type = OperatorType::optr_ExpInParentheses;
            break;
        }
        default: {
            throw InternalError("Operator::Operator invalid token type, realtype: " + std::to_string(t));
        }
    }
}

OperatorType Operator::GetType() const
{
    return this->type;
}

void Operator::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"";
    switch (this->type) {
        case OperatorType::optr_Plus: {
            file << "+";
            break;
        }
        case OperatorType::optr_UnaryMinus:
        case OperatorType::optr_Minus: {
            file << "-";
            break;
        }
        case OperatorType::optr_Multiply: {
            file << "*";
            break;
        }
        case OperatorType::optr_Divide: {
            file << "/";
            break;
        }
        case OperatorType::optr_Concat: {
            file << ".";
            break;
        }
        case OperatorType::optr_Assign: {
            file << "=";
            break;
        }
        case OperatorType::optr_Equal: {
            file << "==";
            break;
        }
        case OperatorType::optr_NotEqual: {
            file << "!=";
            break;
        }
        case OperatorType::optr_Less: {
            file << "<";
            break;
        }
        case OperatorType::optr_LessEqual: {
            file << "<=";
            break;
        }
        case OperatorType::optr_Greater: {
            file << ">";
            break;
        }
        case OperatorType::optr_GreaterEqual: {
            file << ">=";
            break;
        }
        case OperatorType::optr_And: {
            file << "&&";
            break;
        }
        case OperatorType::optr_Or: {
            file << "||";
            break;
        }
        case OperatorType::optr_Not: {
            file << "!";
            break;
        }
        default: {
            throw InternalError("Operator::PrintTree invalid operator type: " + std::to_string(this->type));
        }
    }
    file << "\"];\n";
}