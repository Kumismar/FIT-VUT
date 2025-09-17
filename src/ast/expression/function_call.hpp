/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 18:10
 * @ Modified time: 2024-05-01 11:26
 */

#pragma once

#include "expression.hpp"
#include "operand.hpp"
#include <string>
#include <vector>


/**
 * @brief A class representing a function call in the AST.
 */
class FunctionCall : public Operand
{
private:
    /**
     * @brief Name of the function.
     */
    std::string name;

    /**
     * @brief Arguments of the function.
     */
    std::vector<Expression*> arguments;

public:
    /**
     * @brief Create FunctionCall and set node and statement type.
     */
    FunctionCall();

    /**
     * @brief A destructor.
     */
    ~FunctionCall() override;

    /**
     * @brief Print the tree of the function call.
     * @param file File to write to.
     * @param id ID of the node.
     * @param parentId ID of the parent node.
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
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};