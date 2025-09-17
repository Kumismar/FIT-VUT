/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-05-01 12:42
 */

#include "declaration.hpp"
#include "internal_error.hpp"
#include "nonterminal.hpp"
#include "statement.hpp"
#include "token.hpp"
#include "variable.hpp"

Declaration::Declaration()
{
    this->nodeType = NodeType::nodeStatement;
}

Declaration::~Declaration()
{
    delete this->left;
    delete this->right;
}

void Declaration::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"Declaration\"];\n";

    this->left->PrintTree(file, id, currentId);

    int assignId = ++id;
    file << "node" << currentId << "-> node" << assignId << "\n";
    file << "node" << assignId << "[label=\"=\"];\n";
    id++;

    this->right->PrintTree(file, id, currentId);
}

void Declaration::ProcessToken(Token& token)
{
    if (token == TokenType::t_Variable) {
        this->left = new Variable(token);
    }
}

void Declaration::LinkNode(ASTNode* node, Nonterminal& nt)
{
    switch (nt.GetNonterminalType()) {
        case NonterminalType::nt_Expression: {
            auto* tmp = dynamic_cast<Expression*>(node);
            if (tmp == nullptr) {
                throw InternalError("Declaration::LinkNode (case nt_Expression) invalid type: " + std::string(typeid(*node).name()));
            }

            this->right = tmp;
            break;
        }
        default: {
            return;
        }
    }
}
