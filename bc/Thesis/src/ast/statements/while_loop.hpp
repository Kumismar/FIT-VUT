/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:41
 * @ Modified time: 2024-04-17 16:12
 */

#pragma once

#include "expression.hpp"
#include "statement.hpp"
#include "statement_list.hpp"

/**
 * @brief A class representing a while loop in the AST.
 */
class WhileLoop : public Statement
{
private:
    /**
     * @brief The condition of the while loop.
     */
    Expression* condition = nullptr;

    /**
     * @brief The sequence of statements (body) of the loop.
     */
    StatementList* body = nullptr;

public:
    /**
     * @brief A constructor for the loop. Sets node type.
     */
    WhileLoop();

    /**
     * @brief A destructor of the while loop.
     */
    ~WhileLoop() override;

    /**
     * @brief Print information about the node to the output .dot file.
     * @param file The output file.
     * @param id The id of the node.
     * @param parentId The id of the parent node.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;

    /**
     * @brief Link a node to the AST.
     * @details This method is called on current AST context.
     * The AST nodes are linked to each other.
     * @param node The node to link.
     * @param nt The nonterminal to link.
     */
    void PrintTree(std::ofstream& file, int& id, int parentId) override;
};