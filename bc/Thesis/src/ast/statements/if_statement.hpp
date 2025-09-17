/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:16
 * @ Modified time: 2024-05-04 10:13
 */

#pragma once

#include "elseif_statement.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include <vector>

/**
 * @brief A class representing if statement in the AST.
 */
class IfStatement : public Statement
{
private:
    /**
     * @brief A flag indicating if the if statement is currently processing an else block.
     */
    bool processingElse = false;

    /**
     * @brief The condition of the if statement.
     */
    Expression* condition = nullptr;

    /**
     * @brief The first sequence of statements (if body) of the if statement.
     */
    StatementList* ifBody = nullptr;

    /**
     * @brief The list of elseif statements.
     */
    std::vector<ElseifStatement*> elseifs;

    /**
     * @brief The second sequence of statements (else body) of the if statement.
     */
    StatementList* elseBody = nullptr;

public:
    /**
     * @brief Create IfStatement and set statement type.
     */
    IfStatement();

    /**
     * @brief An overriden destructor from Statement.
     */
    ~IfStatement() override;

    /**
     * @brief Print information about the node to the output .dot file.
     * @param file The output file.
     * @param id The id of the node.
     * @param parentId The id of the parent node.
     */
    void PrintTree(std::ofstream& file, int& id, int parentId) override;

    /**
     * @brief Processes a token.
     * @details This method is called on current AST context.
     * The AST nodes gradually add information to themselves.
     * @param token The token to be processed.
     */
    void ProcessToken(Token& token) override;

    /**
     * @brief Links a node to the current context based on type of nonterminal.
     * @param node The node to link.
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};