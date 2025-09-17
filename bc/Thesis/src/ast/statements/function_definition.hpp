/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 18:30
 * @ Modified time: 2024-04-17 17:02
 */

#pragma once

#include "statement.hpp"
#include "statement_list.hpp"
#include "token.hpp"
#include <string>

/**
 * @brief A class representing a parameter in the function definition.
 */
class Parameter
{
private:
    std::string name;
    TokenType type;
    static std::string typeToString(TokenType t);

public:
    void SetType(TokenType type);
    void SetName(const std::string& name);
    void PrintTree(std::ofstream& file, int& id, int parentId);
};

/**
 * @brief A class representing a function definition in the AST.
 */
class FunctionDefinition : public Statement
{
private:
    /**
     * @brief The current parameter being processed.
     */
    Parameter* currentParam = nullptr;

    /**
     * @brief The name of the function.
     */
    std::string name;

    /**
     * @brief The return type of the function.
     */
    DataType returnType;

    /**
     * @brief A list of parameters of the function.
     */
    std::vector<Parameter*> params;

    /**
     * @brief The list of statements (body) of the function.
     */
    StatementList* body = nullptr;

    /**
     * @brief Check if the token is a type.
     * @param token The token to check.
     * @return True if the token is a type, false otherwise.
     */
    static bool isType(const Token& token);

    /**
     * @brief Set the return type of the function.
     * @param token The token containing return type.
     */
    void setReturnType(const Token& token);

    /**
     * @brief Convert the data type to string.
     * @param t The data type to convert.
     * @return data_String representation of the data type.
     */
    static std::string typeToString(DataType t);

public:
    /**
     * @brief Create FunctionDefinition and set statement and node type.
     */
    FunctionDefinition();

    /**
     * @brief An overriden destructor from Statement.
     */
    ~FunctionDefinition() override;

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
     * @brief Link a node to the AST.
     * @details This method is called on current AST context.
     * The AST nodes are linked to each other.
     * @param node The node to link.
     * @param nt The nonterminal to link.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};