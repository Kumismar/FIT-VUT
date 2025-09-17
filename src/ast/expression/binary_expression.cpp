/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-05-01 12:21
 */

#include "binary_expression.hpp"
#include "ast.hpp"
#include "expression.hpp"
#include "token.hpp"

BinaryExpression::BinaryExpression(TokenType t)
{
    this->nodeType = NodeType::nodeExpression;
    AST* ast = AST::GetInstance();
    this->right = ast->GetExpressionContext();
    ast->PopExpressionContext();
    this->left = ast->GetExpressionContext();
    ast->PopExpressionContext();
    this->optr = new Operator(t);
}

BinaryExpression::~BinaryExpression()
{
    delete this->left;
    delete this->optr;
    delete this->right;
}

Expression* BinaryExpression::GetLeft() const
{
    return this->left;
}

Expression* BinaryExpression::GetRight() const
{
    return this->right;
}

void BinaryExpression::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"BinaryExpression\"];\n";

    this->left->PrintTree(file, id, currentId);
    this->optr->PrintTree(file, id, currentId);
    this->right->PrintTree(file, id, currentId);
}
