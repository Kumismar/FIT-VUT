/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-04-16 13:11
 */

#include "function_call.hpp"
#include "internal_error.hpp"

FunctionCall::FunctionCall()
{
    this->nodeType = NodeType::nodeExpression;
}

FunctionCall::~FunctionCall()
{
    if (!this->arguments.empty()) {
        for (Expression* arg: this->arguments) {
            delete arg;
        }
    }
}

void FunctionCall::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"FunctionCall: " << this->name << "\"];\n";

    for (Expression* arg: this->arguments) {
        arg->PrintTree(file, id, currentId);
    }
}
void FunctionCall::ProcessToken(Token& token)
{
    if (token == TokenType::t_FuncName) {
        this->name = token.GetDataString();
    }
}

void FunctionCall::LinkNode(ASTNode* node, Nonterminal& nt)
{
    if (nt == NonterminalType::nt_Expression) {
        auto* tmp = dynamic_cast<Expression*>(node);
        if (tmp == nullptr) {
            throw InternalError("FunctionCall::LinkNode case nt_Expression node isnt expression, real type:" + std::string(typeid(*node).name()));
        }
        this->arguments.push_back(tmp);
    }
}
