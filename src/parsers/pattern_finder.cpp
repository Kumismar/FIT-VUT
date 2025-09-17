/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 16:10
 * @ Modified time: 2024-04-30 13:57
 */

#include "pattern_finder.hpp"
#include "internal_error.hpp"
#include "precedence_symbol.hpp"

PatternFinder::PatternFinder(AnalysisStack& stack)
    : stack(stack)
{
}

Token* PatternFinder::FindFirstToken()
{
    Symbol* tmpExp = nullptr;
    for (Symbol* symb: this->stack) {
        if (symb->GetSymbolType() == SymbolType::symb_Token) {
            tmpExp = symb;
            break;
        }
    }

    auto* toReturn = dynamic_cast<Token*>(tmpExp);
    if (toReturn == nullptr) {
        throw InternalError("First token in stack is not token.\n");
    }

    return toReturn;
}

void PatternFinder::FindFirstRule(Rule& emptyRule)
{
    // push to list until stack.top is precedence symbol '<' or '$'
    for (Symbol* symb: this->stack) {
        switch (symb->GetSymbolType()) {
            case SymbolType::symb_PrecSymbol: {
                auto* tmpSymbol = dynamic_cast<PrecedenceSymbol*>(symb);
                if (tmpSymbol == nullptr) {
                    throw InternalError("PatternFinder::FindFirstRule: Dynamic cast to PrecedenceSymbol* failed - real type:" + std::string(typeid(*symb).name()));
                }

                // if its precendence symbol '<' then just return
                if (*tmpSymbol == PrecedenceType::Push) {
                    return;
                }
                else {
                    throw InternalError("Different precedence symbol than '<' on the stack.\n");
                }
            }
            case SymbolType::symb_Token: {
                auto* tmpToken = dynamic_cast<Token*>(symb);
                if (tmpToken == nullptr) {
                    throw InternalError("PatternFinder::FindFirstRule: Dynamic cast to Token* failed - real type:" + std::string(typeid(*symb).name()));
                }

                // if its token '$' then end of stack has been reached and just return
                if (*tmpToken == TokenType::t_ExpEnd) {
                    break;
                }
                // else its part of rule, push it
                emptyRule.push_front(new Token(*tmpToken));
                break;
            }
            case SymbolType::symb_Nonterminal: {
                // implies for nonterminals as well
                auto* tmpNT = dynamic_cast<Nonterminal*>(symb);
                if (tmpNT == nullptr) {
                    throw InternalError("PatternFinder::FindFirstRule: Dynamic cast to Nonterminal* failed - real type:" + std::string(typeid(*symb).name()));
                }
                emptyRule.push_front(new Nonterminal(*tmpNT));
                break;
            }
            default: {
                throw InternalError("PatternFinder::FindFirstRule: Unexpected type on stack: " + std::string(typeid(*symb).name()) + ".\n");
            }
        }

        if (this->stack.empty()) {
            throw InternalError("ExpStack empty when finding first rule");
        }
    }
}
