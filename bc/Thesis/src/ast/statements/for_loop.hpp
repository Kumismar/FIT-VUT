/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 11:41
 * @ Modified time: 2024-05-04 10:17
 */

#pragma once

#include "declaration.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "statement_list.hpp"

/**
 * @brief Phases of the for loop for ProcessToken method.
 */
typedef enum phase
{
    phase_Init,
    phase_Condition,
    phase_EndExpr,
    phase_Body
} Phase;

/**
 * @brief Type of initialization part in the for loop.
 */
typedef enum initType
{
    init_Declaration,
    init_Expression,
    init_None
} InitType;

/**
 * @brief A union for initialization data. The initialization can be either a declaration or an expression.
 */
typedef union initData
{
    Declaration* decl;
    Expression* expr;
} InitData;

/**
 * @brief A structure for initialization part of the for loop.
 */
typedef struct init {
    InitType type;
    InitData data;
} Initialization;

/**
 * @brief A class representing the for loop.
 */
class ForLoop : public Statement
{
private:
    /**
     * @brief The current phase of the for loop.
     */
    Phase phase = Phase::phase_Init;

    /**
     * @brief The initialization part of the for loop.
     */
    Initialization* init = nullptr;

    /**
     * @brief The condition of the for loop.
     */
    Expression* condition = nullptr;

    /**
     * @brief The ending expression of the for loop that is executed after each iteration.
     */
    Expression* endExpr = nullptr;

    /**
     * @brief The sequence of statements (body) of the for loop.
     */
    StatementList* body = nullptr;

    /**
     * @brief Checks if the next token is a type.
     * @return True if the next token is a type, false otherwise.
     */
    static bool nextIsType();

public:
    /**
     * @brief A constructor for the for loop. Sets node type, creates the initialization structure and sets the phase to phase_Init.
     */
    ForLoop();

    /**
     * @brief A destructor for the for loop. Clears data of the for loop.
     */
    ~ForLoop() override;

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