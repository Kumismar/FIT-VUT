/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:46
 * @ Modified time: 2024-04-16 12:36
 */

#pragma once

#include "expression.hpp"
#include "statement.hpp"

/**
 * @brief A class representing the return statement.
 */
class ReturnStatement : public Statement
{
private:
    /**
     * @brief The expression to return.
     */
    Expression* expr = nullptr;

public:
    /**
     * @brief Create ReturnStatement and set statement type.
     */
    ReturnStatement();

    /**
     * @brief An overriden destructor from Statement.
     */
    ~ReturnStatement() override;

    /**
     * @brief Print information about the node to the output .dot file.
     * @param file The output file.
     * @param id The id of the node.
     * @param parentId The id of the parent node.
     */
    void PrintTree(std::ofstream& file, int& id, int parentId) override;

    /**
     * @brief Link a node to the AST.
     * @details This method is called on current AST context.
     * The AST nodes are linked to each other.
     * @param node The node to link.
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};