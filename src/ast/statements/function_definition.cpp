/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-05 10:05
 * @ Modified time: 2024-05-03 11:13
 */

#include "function_definition.hpp"
#include "internal_error.hpp"
#include "nonterminal.hpp"
#include "statement.hpp"
#include "token.hpp"

void Parameter::SetType(const TokenType t)
{
    this->type = t;
}

void Parameter::SetName(const std::string& n)
{
    this->name = n;
}

void Parameter::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"Parameter\"];\n";

    int paramId = currentId;
    int nameId = ++currentId;
    file << "node" << paramId << " -> node" << nameId << ";\n";
    id++;
    file << "node" << nameId << " [label=\"Name: " << this->name << "\"];\n";

    int typeId = ++currentId;
    file << "node" << paramId << " -> node" << typeId << ";\n";
    id++;
    file << "node" << typeId << " [label=\"Type: " << Parameter::typeToString(this->type) << "\"];\n";
}

FunctionDefinition::FunctionDefinition()
{
    this->nodeType = NodeType::nodeFuncDef;
    this->returnType = DataType::data_None;
}

FunctionDefinition::~FunctionDefinition()
{
    if (currentParam != nullptr) {
        delete currentParam;
        this->params.pop_back();
    }

    if (!this->params.empty()) {
        for (Parameter* param: this->params) {
            delete param;
        }
        this->params.clear();
    }
    delete this->body;
}

void FunctionDefinition::PrintTree(std::ofstream& file, int& id, int parentId)
{
    int currentId = id++;
    file << "node" << parentId << " -> node" << currentId << ";\n";
    file << "node" << currentId << " [label=\"FunctionDefinition\"];\n";

    int functionId = currentId;
    int functionNameId = ++currentId;
    file << "node" << functionId << "-> node" << functionNameId << ";\n";
    id++;
    file << "node" << functionNameId << " [label=\"Name: " << this->name << "\"];\n";

    for (Parameter* param: this->params) {
        param->PrintTree(file, id, functionId);
    }

    currentId = id++;
    int returnTypeId = ++currentId;
    file << "node" << functionId << "-> node" << returnTypeId << ";\n";
    id++;
    file << "node" << returnTypeId << " [label=\"ReturnType: " << FunctionDefinition::typeToString(this->returnType) << "\"];\n";
    this->body->PrintTree(file, id, functionId);
}

void FunctionDefinition::ProcessToken(Token& token)
{
    if (FunctionDefinition::isType(token)) {
        if (**std::next(inputTape.begin()) == TokenType::t_LCurl) { // next token is '{'
            this->setReturnType(token);
        }
        else {
            this->currentParam = new Parameter();
            this->currentParam->SetType(token.GetTokenType());
        }
    }
    else if (token == TokenType::t_Variable) {
        if (this->currentParam == nullptr) {
            throw InternalError("FunctionDefinition::ProcessToken currentParam is nullptr");
        }

        this->currentParam->SetName(token.GetDataString());
        this->params.push_back(this->currentParam);
    }
    else if (token == TokenType::t_FuncName) {
        this->name = token.GetDataString();
    }
}

void FunctionDefinition::LinkNode(ASTNode* node, Nonterminal& nt)
{
    if (nt.GetNonterminalType() == NonterminalType::nt_Statements) {
        auto* tmp = dynamic_cast<StatementList*>(node);
        if (tmp == nullptr) {
            throw InternalError("FunctionDefiniton::LinkNode (case nt_CodeBlock) invalid type: " + std::string(typeid(*node).name()));
        }

        this->body = tmp;
    }
}

bool FunctionDefinition::isType(const Token& token)
{
    return (token == TokenType::t_Int || token == TokenType::t_Float || token == TokenType::t_String || token == t_Bool);
}

void FunctionDefinition::setReturnType(const Token& token)
{
    switch (token.GetTokenType()) {
        case TokenType::t_Int: {
            this->returnType = DataType::data_Int;
            break;
        }
        case TokenType::t_Float: {
            this->returnType = DataType::data_Float;
            break;
        }
        case TokenType::t_Bool: {
            this->returnType = DataType::data_Bool;
            break;
        }
        case TokenType::t_String: {
            this->returnType = DataType::data_String;
            break;
        }
        default: {
            throw InternalError("FunctionDefinition::setReturnType invalid token type: " + token.GetTypeString());
        }
    }
}

std::string Parameter::typeToString(TokenType t)
{
    switch (t) {
        case TokenType::t_Int: {
            return "Int";
        }
        case TokenType::t_Float: {
            return "Float";
        }
        case TokenType::t_Bool: {
            return "Bool";
        }
        case TokenType::t_String: {
            return "String";
        }
        default: {
            throw InternalError("FunctionDefinition::typeToString invalid token type: " + std::to_string(t));
        }
    }
}

std::string FunctionDefinition::typeToString(DataType t)
{
    switch (t) {
        case DataType::data_Int: {
            return "Int";
        }
        case DataType::data_Float: {
            return "Float";
        }
        case DataType::data_Bool: {
            return "Bool";
        }
        case DataType::data_String: {
            return "String";
        }
        default: {
            throw InternalError("FunctionDefinition::typeToString invalid data type: " + std::to_string(t));
        }
    }
}