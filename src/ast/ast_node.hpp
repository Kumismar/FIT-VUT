/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-04 11:00
 * @ Modified time: 2024-05-04 09:29
 */

#pragma once

#include "nonterminal.hpp"
#include "token.hpp"
#include <fstream>

/**
 * @brief An enumeration of node types.
 */
typedef enum nodeType
{
    nodeExpression,
    nodeStatement,
    nodeStList,
    nodeFuncDef
} NodeType;


/**
 * @brief The base class for all nodes in the Abstract Syntax Tree.
 */
class ASTNode
{
protected:
    /**
     * @brief The type of the node.
     */
    NodeType nodeType;

public:
    /**
     * @brief A virtual destructor.
     */
    virtual ~ASTNode() = default;

    /**
     * @brief Processes a token.
     * @details This method is called on current AST context.
     * The AST nodes gradually add information to themselves.
     * @param t The token to process.
     */
    virtual void ProcessToken(Token& t) = 0;

    /**
     * @brief Links a node to the current context based on type of nonterminal.
     * @param node The node to link.
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    virtual void LinkNode(ASTNode* node, Nonterminal& nt) = 0;

    /**
     * @brief Print information about the node to the output .dot file.
     * @param file The output file.
     * @param id Global id for every node.
     * @param parentId The id of the parent node.
     */
    virtual void PrintTree(std::ofstream& file, int& id, int parentId) = 0;

    /**
     * @brief Get the type of the node.
     * @return The type of the node.
     */
    NodeType GetNodeType() const
    {
        return nodeType;
    }
};