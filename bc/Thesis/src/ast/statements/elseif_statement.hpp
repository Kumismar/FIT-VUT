/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:19
 * @ Modified time: 2024-05-04 10:15
 */

#pragma once

#include "expression.hpp"
#include "statement.hpp"
#include "statement_list.hpp"

/**
 * @brief A class representing an elseif statement in the AST.
 */
class ElseifStatement : public Statement
{
private:
    /**
     * @brief The condition of the elseif statement.
     */
    Expression* condition = nullptr;

    /**
     * @brief The sequence of statements (body) of the elseif statement.
     */
    StatementList* body = nullptr;

public:
    /**
     * @brief Create ElseifStatement and set statement type.
     */
    ElseifStatement();

    /**
     * @brief An overriden destructor from Statement.
     */
    ~ElseifStatement() override;

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
     * @param nt The nonterminal to link.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};