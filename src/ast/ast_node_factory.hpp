/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-07 14:27
 * @ Modified time: 2024-05-04 09:35
 */

#pragma once

#include "ast_node.hpp"
#include "expression.hpp"
#include "grammar.hpp"
#include "nonterminal.hpp"
#include "token.hpp"

class ASTNodeFactory
{
private:
    /**
     * @brief Creates an operand AST node.
     * @param rule The rule to create the operand from.
     * @return The created operand.
     */
    static Expression* createOperand(Rule& rule);

    /**
     * @brief Creates a unary expression AST node.
     * @param rule The rule to create the unary expression from.
     * @return The created unary expression.
     */
    static Expression* createUnaryExpression(Rule& rule);

    /**
     * @brief Creates a binary expression AST node.
     * @param rule The rule to create the binary expression from.
     * @return The created binary expression.
     */
    static Expression* createBinaryExpression(Rule& rule);

    /**
     * @brief Checks if given rule is an unary expression.
     * @param rule The rule to check.
     * @return True if the rule is an unary expression, false otherwise.
     */
    static bool isUnaryExpression(Rule& rule);

public:
    /**
     * @brief Creates an AST node from a nonterminal and a token.
     * @details The nonterminal is mainly used to determine the type of the AST node.
     * Sometimes, token is needed to get the node type.
     * @param nt The nonterminal to create the AST node from.
     * @param t The token to create the AST node from.
     * @return The created AST node.
     */
    static ASTNode* CreateASTNode(Nonterminal& nt, Token& t);

    /**
     * @brief Creates an expression AST node from a rule.
     * @details The rule is used to determine the type of the node.
     * @param rule The rule to create the node from.
     * @return The created expression node.
     */
    static Expression* CreateASTNode(Rule& rule);
};