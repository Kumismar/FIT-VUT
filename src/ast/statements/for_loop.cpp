/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-04-17 16:14
 */

#include "for_loop.hpp"
#include "expression.hpp"
#include "internal_error.hpp"
#include "nonterminal.hpp"
#include "statement.hpp"
#include "statement_list.hpp"
#include "token.hpp"

ForLoop::ForLoop()
{
    this->init = new Initialization();
    this->init->type = InitType::init_None;
    this->nodeType = NodeType::nodeStatement;
}

ForLoop::~ForLoop()
{
    if (this->init->type == InitType::init_Declaration) {
        delete this->init->data.decl;
    }
    else if (this->init->type == InitType::init_Expression) {
        delete this->init->data.expr;
    }
    delete this->init;

    delete this->condition;
    delete this->endExpr;
    delete this->body;
}

void ForLoop::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"ForLoop\"];\n";

    if (this->init->type == InitType::init_Declaration) {
        this->init->data.decl->PrintTree(file, id, currentId);
    }
    else if (this->init->type == InitType::init_Expression) {
        this->init->data.expr->PrintTree(file, id, currentId);
    }

    this->condition->PrintTree(file, id, currentId);
    this->endExpr->PrintTree(file, id, currentId);
    this->body->PrintTree(file, id, currentId);
}

void ForLoop::LinkNode(ASTNode* node, Nonterminal& nt)
{
    switch (nt.GetNonterminalType()) {
        case NonterminalType::nt_CodeBlock: {
            auto* tmp = dynamic_cast<StatementList*>(node);
            if (tmp == nullptr) {
                throw InternalError("ForLoop::LinkNode (case nt_CodeBlock) invalid type: " + std::string(typeid(*node).name()));
            }

            this->body = tmp;
            break;
        }
        case NonterminalType::nt_Expression: {
            auto* tmp = dynamic_cast<Expression*>(node);
            if (tmp == nullptr) {
                throw InternalError("ForLoop::LinkNode (case nt_Statement) invalid type: " + std::string(typeid(*node).name()));
            }

            if (this->phase == Phase::phase_Init) {
                this->init->type = InitType::init_Expression;
                this->init->data.expr = tmp;
                this->phase = Phase::phase_Condition;
            }
            else if (this->phase == Phase::phase_Condition) {
                this->condition = tmp;
                this->phase = Phase::phase_EndExpr;
            }
            else if (this->phase == Phase::phase_EndExpr) {
                this->endExpr = tmp;
                this->phase = Phase::phase_Body;
            }
            break;
        }
        case NonterminalType::nt_DeclOrExpr: {
            if (!ForLoop::nextIsType()) {
                return;
            }

            auto* tmp = dynamic_cast<Declaration*>(node);
            if (tmp == nullptr) {
                throw InternalError("ForLoop::LinkNode (case nDeclOrExp) invalid type: " + std::string(typeid(*node).name()));
            }

            this->init->type = InitType::init_Declaration;
            this->init->data.decl = tmp;
            this->phase = Phase::phase_Condition;
            break;
        }
        default: {
            return;
        }
    }
}

bool ForLoop::nextIsType()
{
    Token* next = inputTape.front();
    return (*next == TokenType::t_Int || *next == TokenType::t_Float || *next == TokenType::t_Bool || *next == TokenType::t_String);
}
