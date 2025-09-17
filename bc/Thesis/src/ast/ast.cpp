/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-04-29 11:56
 */

#include "ast.hpp"
#include "internal_error.hpp"
#include "statement_list.hpp"
#include <filesystem>

AST* AST::instance = nullptr;

AST::AST()
{
    this->root = new StatementList();
    this->nodeContext.push(this->root);
}

AST::~AST()
{
    while (!this->nodeContext.empty()) {
        // All the objects in AST were already deleted, just pop the pointers
        AST::instance->nodeContext.pop();
    }
    if (this->root != nullptr) {
        delete this->root;
        this->root = nullptr;
    }

    while (!this->exprContext.empty()) {
        delete this->exprContext.top();
        AST::instance->exprContext.pop();
    }

    if (this->file == nullptr) {
        return;
    }
    this->file->close();
    delete this->file;
}

ASTNode* AST::GetCurrentContext()
{
    if (this->turnedOff) {
        return nullptr;
    }

    if (this->nodeContext.empty()) {
        return nullptr;
    }
    return this->nodeContext.top();
}

Expression* AST::GetExpressionContext()
{
    if (this->turnedOff) {
        return nullptr;
    }

    if (this->exprContext.empty()) {
        throw InternalError("AST::GetExpressionContext stack empty\n");
    }
    return this->exprContext.top();
}


void AST::PopContext()
{
    if (this->turnedOff) {
        return;
    }

    this->nodeContext.pop();
}

void AST::PushContext(ASTNode* node)
{
    if (this->turnedOff) {
        return;
    }

    this->nodeContext.push(node);
}

void AST::PopExpressionContext()
{
    if (this->turnedOff) {
        return;
    }

    this->exprContext.pop();
}

void AST::PushExpressionContext(Expression* exp)
{
    if (this->turnedOff) {
        return;
    }

    this->exprContext.push(exp);
}

void AST::SetTreeFlag()
{
    if (this->turnedOff) {
        return;
    }

    this->printTreeFlag = true;
}

void AST::PrintTree()
{
    if (!this->printTreeFlag || this->turnedOff) {
        return;
    }

    namespace fs = std::filesystem;
    fs::path filePath = fs::path(__FILE__).parent_path().parent_path().parent_path() / "tree.dot";

    this->file = new std::ofstream();
    this->file->open(filePath, std::ios::out | std::ios::trunc);
    if (!this->file->is_open()) {
        throw InternalError("Couldn't open tree.dot file\n");
    }
    int id = 0;
    *this->file << "digraph G {\n";
    *this->file << "node" << id << " [label=\"Program\"];\n";
    this->root->PrintTree(*this->file, ++id, 0);
    *this->file << "}" << std::endl;
}

AST* AST::GetInstance()
{
    if (AST::instance == nullptr) {
        AST::instance = new AST();
    }
    return AST::instance;
}

void AST::Cleanup()
{
    if (AST::instance == nullptr) {
        return;
    }
    delete AST::instance;
    AST::instance = nullptr;
}

void AST::TurnOff()
{
    this->turnedOff = true;
}

bool AST::IsTurnedOff() const
{
    return this->turnedOff;
}
