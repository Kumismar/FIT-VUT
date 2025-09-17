/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 18:14
 * @ Modified time: 2024-04-16 13:56
 */

#pragma once

#include "operand.hpp"
#include "token.hpp"

/**
 * @brief A class representing a constant in the AST.
 */
class Constant : public Operand
{
public:
    /**
     * @brief Create Constant and set node type.
     * @param t Token of the constant.
     */
    explicit Constant(Token& t);

    /**
     * @brief A destructor.
     */
    ~Constant() override;

    /**
     * @brief Print the tree of the constant.
     * @param file File to write to.
     * @param id ID of the node.
     * @param parentId ID of the parent node.
     */
    void PrintTree(std::ofstream& file, int& id, int parentId) override;
};