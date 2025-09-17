/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-16 11:45
 * @ Modified time: 2024-05-01 12:10
 */

#include "unary_expression.hpp"
#include "ast.hpp"

UnaryExpression::UnaryExpression(TokenType t)
{
    this->nodeType = NodeType::nodeExpression;
    this->operand = AST::GetInstance()->GetExpressionContext();
    AST::GetInstance()->PopExpressionContext();
    this->optr = new Operator(t);
}

UnaryExpression::~UnaryExpression()
{
    delete this->operand;
    delete this->optr;
}

Expression* UnaryExpression::GetOperand() const
{
    return this->operand;
}

void UnaryExpression::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;

    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"UnaryExpression\"];\n";

    if (this->optr->GetType() == OperatorType::optr_ExpInParentheses) {
        int uExpId = currentId;
        int lParId = currentId + 1;
        id++;

        file << "node" << uExpId << " -> node" << lParId << ";\n";
        file << "node" << lParId << " [label=\"(\"];\n";
        this->operand->PrintTree(file, id, uExpId);

        int rParId = id;
        id++;
        file << "node" << uExpId << " -> node" << rParId << ";\n";
        file << "node" << rParId << " [label=\")\"];\n";
        return;
    }

    this->optr->PrintTree(file, id, currentId);
    this->operand->PrintTree(file, id, currentId);
}