/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 10:29
 * @ Modified time: 2024-05-03 11:20
 */

#pragma once

#include "ast_node.hpp"
#include "nonterminal.hpp"

/**
 * @brief An abstract class representing a statement in the AST.
 */
class Statement : public ASTNode
{
public:
    /**
     * @brief A virtual destructor.
     */
    ~Statement() override = default;

    /**
     * @brief Processes a token.
     * @details This method is called on current AST context.
     * The AST nodes gradually add information to themselves.
     * @param t The token to process.
     */
    void ProcessToken(Token& t) override {}

    /**
     * @brief Links a node to the current context based on type of nonterminal.
     * @param node The node to link.
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override {}
};