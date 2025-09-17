/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:44
 * @ Modified time: 2024-05-04 09:38
 */

#pragma once

#include "ast_node.hpp"
#include "expression.hpp"
#include "operand.hpp"
#include "statement.hpp"

/**
 * @brief A class representing a declaration in the AST.
 */
class Declaration : public Statement
{
private:
    /**
     * @brief The left side of the declaration. Usually a variable, but isn't semantically checked yet.
     */
    Operand* left = nullptr;

    /**
     * @brief The right side of the declaration.
     */
    Expression* right = nullptr;

public:
    /**
     * @brief Create Declaration and set statement type.
     */
    Declaration();

    /**
     * @brief An overriden destructor from Statement.
     */
    ~Declaration() override;

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
     * @param token The token to process.
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