/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 10:26
 * @ Modified time: 2024-05-04 09:21
 */


#pragma once

#include "ast_node.hpp"
#include "statement_list.hpp"
#include <fstream>
#include <stack>

/**
 * @brief The main class for the Abstract Syntax Tree.
 * @details The class is implemented as singleton, the instance is created and accessed through the GetInstance method.
 */
class AST
{
private:
    /**
     * @brief The instance of the AST.
     */
    static AST* instance;

    /**
     * @brief The root of the AST.
     */
    StatementList* root = nullptr;

    /**
     * @brief Stack of context nodes.
     * The nodes are connected with LinkNode method in the ASTNode class.
     */
    std::stack<ASTNode*> nodeContext;

    /**
     * @brief Stack of expression nodes.
     * The nodes are connected when reducing the expression.
     */
    std::stack<Expression*> exprContext;

    /**
     * @brief The output file for the AST.
     */
    std::ofstream* file = nullptr;

    /**
     * @brief A flag indicating that the AST should be printed.
     */
    bool printTreeFlag = false;

    /**
     * @brief A flag indicating whether the AST is turned off.
     */
    bool turnedOff = false;

    /**
     * @brief The constructor of the AST.
     * @details The constructor is private because the class is implemented as a singleton.
     * Creates its root and opens the output file.
     */
    AST();

    /**
     * @brief The destructor of the AST.
     * @details Closes the output file.
     */
    ~AST();

public:
    /**
     * @brief Get a pointer to current context node in which the AST is being built.
     * @return The pointer to the current context node.
     */
    ASTNode* GetCurrentContext();

    /**
     * @brief Get a pointer to current expression context node in which the AST is being built.
     * @return The pointer to the current expression context node.
     */
    Expression* GetExpressionContext();

    /**
     * @brief Pop the current context node from the stack.
     */
    void PopContext();

    /**
     * @brief Push a new context node to the stack.
     * @param node The node to be pushed.
     */
    void PushContext(ASTNode* node);

    /**
     * @brief Pop the current expression context node from the stack.
     */
    void PopExpressionContext();

    /**
     * @brief Push a new expression context node to the stack.
     * @param exp The expression node to be pushed.
     */
    void PushExpressionContext(Expression* exp);

    /**
     * @brief Sets tree printing flag to true if "-t" option was used.
     */
    void SetTreeFlag();

    /**
     * @brief Traverse the AST and make a .dot file out of it.
     */
    void PrintTree();

    /**
     * @brief Get an AST singleton instance.
     */
    static AST* GetInstance();

    /**
     * @brief Clean all the resources used by the AST.
     */
    static void Cleanup();

    /**
     * @brief Turn off the AST.
     */
    void TurnOff();

    /**
     * @brief Check if the AST is turned off.
     * @return True if the AST is turned off, false otherwise.
     */
    bool IsTurnedOff() const;
};