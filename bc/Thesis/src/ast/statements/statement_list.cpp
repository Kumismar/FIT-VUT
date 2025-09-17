/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-07 16:18
 * @ Modified time: 2024-05-01 12:55
 */


#include "statement_list.hpp"
#include "expression.hpp"
#include "internal_error.hpp"
#include "statement.hpp"

StatementList::StatementList()
{
    this->nodeType = NodeType::nodeStList;
}

StatementList::~StatementList()
{
    if (this->statements.empty()) {
        return;
    }

    for (StatementOrExpression* item: this->statements) {
        if (item->type == StOrExpType::soe_Statement) {
            delete item->data.statement;
        }
        else {
            delete item->data.expression;
        }
        delete item;
    }
    this->statements.clear();
}

void StatementList::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"StatementList\"];\n";

    for (StatementOrExpression* item: this->statements) {
        if (item->type == StOrExpType::soe_Statement) {
            item->data.statement->PrintTree(file, id, currentId);
        }
        else if (item->type == StOrExpType::soe_Expression) {
            item->data.expression->PrintTree(file, id, currentId);
        }
        else {
            item->data.statementList->PrintTree(file, id, currentId);
        }
    }
}

void StatementList::ProcessToken(Token& token)
{
}

void StatementList::LinkNode(ASTNode* node, Nonterminal& nt)
{
    auto* tmp = new StatementOrExpression();
    if (nt.GetNonterminalType() == NonterminalType::nt_Expression) {
        auto* tmpNode = dynamic_cast<Expression*>(node);
        if (tmpNode == nullptr) {
            throw InternalError("StatementList::LinkNode (case nt_Expression) invalid type: " + std::string(typeid(*node).name()));
        }

        tmp->type = StOrExpType::soe_Expression;
        tmp->data.expression = tmpNode;
    }
    else if (nt.GetNonterminalType() == NonterminalType::nt_Statements) {
        // This is the case of rule <statements> -> <statement> <statements> where new node is generated for each <statements>
        // Since there has to be <codeblock> between in order to have nested statements, we can just ignore it.
        return;
    }
    else if (nt.GetNonterminalType() == NonterminalType::nt_CodeBlock) {
        auto* tmpStmtList = dynamic_cast<StatementList*>(node);
        if (tmpStmtList == nullptr) {
            throw InternalError("StatementList::LinkNode (case nt_CodeBlock) invalid type: " + std::string(typeid(*node).name()));
        }

        tmp->type = StOrExpType::soe_StList;
        tmp->data.statementList = tmpStmtList;
    }
    else {
        auto* tmpStmt = dynamic_cast<Statement*>(node);
        if (tmpStmt == nullptr) {
            throw InternalError("StatementList::LinkNode (case nt_Statement) invalid type: " + std::string(typeid(*node).name()));
        }

        tmp->type = StOrExpType::soe_Statement;
        tmp->data.statement = tmpStmt;
    }
    this->statements.push_back(tmp);
}