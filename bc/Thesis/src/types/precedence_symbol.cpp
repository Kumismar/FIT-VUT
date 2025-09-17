/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-03-23 19:09
 */

#include "precedence_symbol.hpp"
#include "internal_error.hpp"
#include "stack_item.hpp"

PrecedenceSymbol::PrecedenceSymbol(const PrecedenceType type)
    : precType(type)
{
    this->symbType = SymbolType::symb_PrecSymbol;
}

PrecedenceSymbol::PrecedenceSymbol(const PrecedenceSymbol& old)
    : precType(old.GetPrecedenceType())
{
    this->symbType = SymbolType::symb_PrecSymbol;
}

PrecedenceType PrecedenceSymbol::GetPrecedenceType() const
{
    return this->precType;
}

std::string PrecedenceSymbol::GetTypeString() const
{
    switch (this->precType) {
        case PrecedenceType::Push: {
            return "<";
        }
        case PrecedenceType::Reduce: {
            return ">";
        }
        case PrecedenceType::Equal: {
            return "=";
        }
        default: {
            throw InternalError("Invalid PrecedenceSymbol type in PrecedenceSymbol::GetTypeString()\n");
        }
    }
}

bool PrecedenceSymbol::operator==(const PrecedenceType& type) const
{
    return this->precType == type;
}
bool PrecedenceSymbol::operator==(const PrecedenceSymbol& other) const
{
    return this->precType == other.GetPrecedenceType();
}

Symbol* PrecedenceSymbol::Clone() const
{
    return new PrecedenceSymbol(*this);
}