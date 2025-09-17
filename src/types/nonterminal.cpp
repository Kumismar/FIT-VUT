/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-04-08 11:25
 */

#include "nonterminal.hpp"
#include "internal_error.hpp"
#include "stack_item.hpp"

Nonterminal::Nonterminal(const NonterminalType t)
    : ntType(t)
{
    this->symbType = SymbolType::symb_Nonterminal;
}

Nonterminal::Nonterminal(const Nonterminal& old)
    : ntType(old.GetNonterminalType())
{
    this->symbType = SymbolType::symb_Nonterminal;
}

bool Nonterminal::operator==(const NonterminalType& other) const
{
    return (this->ntType == other);
}

bool Nonterminal::operator==(const Nonterminal& other) const
{
    return (this->ntType == other.GetNonterminalType());
}

NonterminalType Nonterminal::GetNonterminalType() const
{
    return this->ntType;
}

std::string Nonterminal::GetTypeString() const
{
    switch (this->ntType) {
        case NonterminalType::nt_Program:
            return "<program>";
        case NonterminalType::nt_StatementList:
            return "<statementList>";
        case NonterminalType::nt_Statement:
            return "<statement>";
        case NonterminalType::nt_If2:
            return "<if2>";
        case NonterminalType::nt_DeclOrExpr:
            return "<declOrExpr>";
        case NonterminalType::nt_ReturnExp:
            return "<returnExp>";
        case NonterminalType::nt_FunctionDef:
            return "<functionDef>";
        case NonterminalType::nt_Params:
            return "<params>";
        case NonterminalType::nt_Params2:
            return "<params2>";
        case NonterminalType::nt_Expression:
            return "<expression>";
        case NonterminalType::nt_Args:
            return "<args>";
        case NonterminalType::nt_Args2:
            return "<args2>";
        case NonterminalType::nt_CodeBlock:
            return "<codeBlock>";
        case NonterminalType::nt_Statements:
            return "<statements>";
        case NonterminalType::nt_VoluntaryType:
            return "<voluntaryType>";
        case NonterminalType::nt_Type:
            return "<type>";
        case nStop:
            return "";
        default:
            throw InternalError("Unknown nonterminal type\n");
    }
}

Nonterminal* Nonterminal::Clone() const
{
    return new Nonterminal(*this);
}