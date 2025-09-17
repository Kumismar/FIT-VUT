/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 10:29
 * @ Modified time: 2024-05-01 12:55
 */

#pragma once

#include "expression.hpp"
#include "statement.hpp"
#include <fstream>
#include <vector>


/**
 * @brief An enumeration for the type of the statement or expression.
 */
typedef enum stOrExpType
{
    soe_Statement,
    soe_Expression,
    soe_StList
} StOrExpType;

// Forward declaration for circular dependency
class StatementList;

/**
 * @brief A union for statement data. The statement can be either a statement, an expression or a statement list.
 */
typedef union statementData
{
    Statement* statement;
    Expression* expression;
    StatementList* statementList;
} StOrExpData;

/**
 * @brief A structure representing a statement or an expression.
 */
typedef struct statementOrExpression {
    StOrExpType type;
    StOrExpData data;
} StatementOrExpression;

/**
 * @brief A class representing a list of statements in the AST.
 */
class StatementList : public ASTNode
{
private:
    /**
     * @brief A list of statements in the statement list.
     */
    std::vector<StatementOrExpression*> statements;

public:
    /**
     * @brief Create StatementList and set node type.
     */
    StatementList();

    /**
     * @brief An overriden destructor from ASTNode.
     */
    ~StatementList() override;

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
     * @param nt Nonterminal used to determine if the node will be linked.
     * Is used to dynamically cast the node to the correct type.
     */
    void LinkNode(ASTNode* node, Nonterminal& nt) override;
};