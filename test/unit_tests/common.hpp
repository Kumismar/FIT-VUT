/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-29 09:17
 * @ Modified time: 2024-05-02 22:06
 */

#pragma once

#include "ast.hpp"
#include "ast_node_factory.hpp"
#include "grammar.hpp"
#include "statement.hpp"
#include "token.hpp"
#include <cassert>
#include <vector>


bool handlesEqual(Rule& r1, Rule& r2);
void cleanRule(Rule& rule);

Token* sampleToken(TokenType type = t_Const, DataType dType = DataType::data_Int, Value value = Value{ .intVal = 0 });
void createOperandAndPush(Token* t);
Expression* createUnaryExpression(TokenType type);
Expression* createBinaryExpression(TokenType type);