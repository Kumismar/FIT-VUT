/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-05-04 15:12
 */

#include "token.hpp"
#include "internal_error.hpp"
#include "stack_item.hpp"
#include <string>

InputTape inputTape;

Token::Token(const TokenType t)
    : type(t)
{
    if (t == TokenType::t_End) {
        this->data.type = DataType::data_String;
        this->data.value.stringVal = new std::string("EOF");
    }
    else {
        this->data.type = DataType::data_None;
    }
    this->symbType = SymbolType::symb_Token;
    this->lineno = yyget_lineno();
}


Token::Token(const Token& other)
    : type(other.GetTokenType())
{
    this->symbType = SymbolType::symb_Token;
    this->lineno = other.GetLineNo();
    if (other.data.type == DataType::data_String) {
        this->data.type = DataType::data_String;
        this->data.value.stringVal = new std::string(*other.data.value.stringVal);
    }
    else {
        this->data = other.data;
    }
}

Token::~Token()
{
    if (this->data.type == DataType::data_String && this->data.value.stringVal != nullptr) {
        delete this->data.value.stringVal;
    }
}

bool Token::operator==(const Token& other) const
{
    return this->type == other.GetTokenType();
}

bool Token::operator!=(const Token& other) const
{
    return !(*this == other);
}

bool Token::operator==(const TokenType& other) const
{
    return (this->type == other);
}

bool Token::operator!=(const TokenType& other) const
{
    return !(*this == other);
}

TokenType Token::GetTokenType() const
{
    return this->type;
}

void Token::SetTokenType(TokenType t)
{
    this->type = t;
}

void Token::SetData(DataType dtype)
{
    this->lineno = yyget_lineno();
    std::string tmp = yytext;

    this->data.type = dtype;
    switch (dtype) {
        case DataType::data_Int:
            this->data.value.intVal = std::stoi(tmp);
            break;
        case DataType::data_Float:
            this->data.value.floatVal = std::stof(tmp);
            break;
        case DataType::data_String:
            this->data.value.stringVal = new std::string(tmp);
            break;
        case DataType::data_Bool:
            this->data.value.boolVal = (tmp == "true");
            break;
        case DataType::data_None:
            break;
    }
}

void Token::SetData(DataType dtype, Value value)
{
    this->lineno = 0;
    this->data.type = dtype;
    this->data.value = value;
}

std::string Token::GetTypeString() const
{
    switch (this->type) {
        case TokenType::t_If:
            return "if";
        case TokenType::t_While:
            return "while";
        case TokenType::t_For:
            return "for";
        case TokenType::t_Return:
            return "return";
        case TokenType::t_Semi:
            return ";";
        case TokenType::t_Elseif:
            return "elseif";
        case TokenType::t_Else:
            return "else";
        case TokenType::t_LPar:
            return "(";
        case TokenType::t_RPar:
            return ")";
        case TokenType::t_Function:
            return "function";
        case TokenType::t_FuncName:
            return "funcName";
        case TokenType::t_Variable:
            return "variable";
        case TokenType::t_Comma:
            return ",";
        case TokenType::t_Colon:
            return ":";
        case TokenType::t_Plus:
            return "+";
        case TokenType::t_UnMinus:
            return "un-";
        case TokenType::t_Minus:
            return "-";
        case TokenType::t_Mul:
            return "*";
        case TokenType::t_Div:
            return "/";
        case TokenType::t_Concat:
            return ".";
        case TokenType::t_And:
            return "&&";
        case TokenType::t_Or:
            return "||";
        case TokenType::t_Eq:
            return "==";
        case TokenType::t_NEq:
            return "!=";
        case TokenType::t_Greater:
            return ">";
        case TokenType::t_Less:
            return "<";
        case TokenType::t_GEq:
            return ">=";
        case TokenType::t_LEq:
            return "<=";
        case TokenType::t_Excl:
            return "!";
        case TokenType::t_Assign:
            return "=";
        case TokenType::t_Const:
            return "constant";
        case TokenType::t_Int:
            return "int";
        case TokenType::t_Float:
            return "float";
        case TokenType::t_String:
            return "string";
        case TokenType::t_Bool:
            return "bool";
        case TokenType::t_LCurl:
            return "{";
        case TokenType::t_RCurl:
            return "}";
        case TokenType::t_Eps:
            return "epsilon";
        case TokenType::t_End:
            return "EOF";
        case TokenType::t_ExpEnd:
            return "EOE";
        case TokenType::t_FuncConst:
            return "funcConst";
        case TokenType::t_FuncEnd:
            return "funcEnd";
        default:
            throw InternalError("Unknown token type in Token::GetTypeString()\n");
    }
}

std::string Token::GetDataString() const
{
    switch (this->data.type) {
        case data_Int:
            return std::to_string(this->data.value.intVal);
        case data_Float:
            return std::to_string(this->data.value.floatVal);
        case data_String:
            return *this->data.value.stringVal;
        case data_Bool:
            return this->data.value.boolVal ? "true" : "false";
        case data_None:
            return this->GetTypeString();
        default:
            throw InternalError("Unknown data type in Token::GetDataString()\n");
    }
}

unsigned int Token::GetLineNo() const
{
    return this->lineno;
}

Token* Token::Clone() const
{
    return new Token(*this);
}

DataType Token::GetDataType() const
{
    return this->data.type;
}

Value Token::GetData() const
{
    return this->data.value;
}


void Token::AddToken(TokenType ttype, DataType dtype)
{
    extern TokenType previousToken;
    auto* token = new Token(ttype);
    token->SetData(dtype);
    inputTape.push_back(token);
    previousToken = ttype;
}