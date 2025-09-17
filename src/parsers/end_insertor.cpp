/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 15:42
 * @ Modified time: 2024-04-28 21:18
 */

#include "end_insertor.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

void EndInsertor::InsertExpressionEnd()
{
    auto token = inputTape.begin();
    while (token != inputTape.end()) {
        // skip operand
        EndInsertor::skipOperand(token);

        // if the operand was the last one, insert expEnd behind
        if (!EndInsertor::isOperator(**token)) {
            break;
        }
        // skip operator
        token++;
    }

    if (token == inputTape.end()) {
        throw SyntaxError("Missing token(s).\n");
    }

    inputTape.insert(token, new Token(TokenType::t_ExpEnd));
}

void EndInsertor::InsertFunctionEnd()
{
    // go through inputTape and insert t_FuncEnd after the first right parenthesis matched with left parenthesis
    int counter = 0;
    for (auto token = inputTape.begin(); token != inputTape.end(); token++) {
        if (**token == TokenType::t_LPar) {
            counter++;
        }
        else if (**token == TokenType::t_RPar) {
            counter--;
        }

        if (counter == 0 && **token == TokenType::t_RPar) {
            inputTape.insert(++token, new Token(TokenType::t_FuncEnd));
            return;
        }
    }
}

void EndInsertor::skipOperand(InputTape::iterator& token)
{
    // skip unary operators
    while (**token == TokenType::t_UnMinus || **token == TokenType::t_Excl) {
        token++;
        if (token == inputTape.end()) {
            throw SyntaxError("Missing token(s).\n");
        }
    }

    if (**token == TokenType::t_LPar) {
        EndInsertor::skipOperandInParentheses(token);
        return;
    }

    if (**token == TokenType::t_FuncName) {
        EndInsertor::skipFunctionCall(token);
        return;
    }

    if (**token != TokenType::t_Const && **token != TokenType::t_Variable) {
        throw SyntaxError("Invalid token.\n");
    }

    token++;
    if (token == inputTape.end()) {
        throw SyntaxError("Missing token(s).\n");
    }
}

void EndInsertor::skipFunctionCall(InputTape::iterator& token)
{
    int counter = 0;

    for (token++; (token != inputTape.end() && **token != t_ExpEnd); token++) {
        if (**token == t_LPar) {
            counter++;
        }
        else if (**token == t_RPar) {
            counter--;
        }

        if (counter == 0) {
            token++;
            return;
        }
    }

    if (token == inputTape.end() || **token == t_ExpEnd) {
        throw SyntaxError("Invalid token.\n");
    }
}

void EndInsertor::skipOperandInParentheses(InputTape::iterator& token)
{
    int counter = 0;
    while (token != inputTape.end()) {
        if (**token == t_LPar) {
            counter++;
        }
        else if (**token == t_RPar) {
            counter--;
        }

        if (counter == 0) {
            break;
        }

        token++;
    }

    token++;
    if (token == inputTape.end()) {
        throw SyntaxError("Missing token(s).\n");
    }
}

bool EndInsertor::isOperator(Token& token)
{
    return (token == t_Plus || token == t_Minus || token == t_Mul || token == t_Div || token == t_Concat ||
            token == t_And || token == t_Or ||
            token == t_Eq || token == t_NEq || token == t_Less || token == t_Greater || token == t_LEq || token == t_GEq ||
            token == t_Assign);
}