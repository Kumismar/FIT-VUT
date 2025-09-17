/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 10:29
 * @ Modified time: 2024-05-01 12:40
 */

#pragma once

#include "ast_node.hpp"
#include "nonterminal.hpp"

/**
 * @brief A class representing an expression in the AST.
 */
class Expression : public ASTNode
{
public:
    /**
     * @brief A destructor.
     */
    ~Expression() override = default;

    /**
     * @brief Processes a token.
     * @details This method is called on current AST context.
     * The AST nodes gradually add information to themselves.
     * @param token The token to process.
     */
    void ProcessToken(Token& token) override
    {
    }

    /**
     * @brief Links a node to the current context based on type of nonterminal.
     * @param node The node to link.
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override
    {
    }
};