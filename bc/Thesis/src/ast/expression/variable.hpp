/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 18:21
 * @ Modified time: 2024-05-01 12:40
 */

#pragma once

#include "operand.hpp"
#include "token.hpp"

/**
 * @brief A class representing a variable in the AST.
 */
class Variable : public Operand
{
private:
    /**
     * @brief Name of the variable.
     */
    std::string name;

public:
    /**
     * @brief Create Variable and set node type.
     * @param token Token with the variable name.
     */
    explicit Variable(Token& token);

    /**
     * @brief A destructor.
     */
    ~Variable() override = default;

    /**
     * @brief Print the tree of the unary expression.
     * @param file File to write to.
     * @param id ID of the node.
     * @param parentId ID of the parent node.
     */
    void PrintTree(std::ofstream& file, int& id, int parentId) override;
};