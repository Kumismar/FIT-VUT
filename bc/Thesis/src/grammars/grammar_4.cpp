/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:33
 */

#include "grammar_4.hpp"
#include "nonterminal.hpp"
#include "token.hpp"
#include <list>

const std::vector<Rule> Grammar4::rightSideRules = {
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Plus), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Minus), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Mul), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Div), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Concat), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_And), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Or), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Eq), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_NEq), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Greater), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Less), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_GEq), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_LEq), new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression), new Token(t_Assign), new Nonterminal(NonterminalType::nt_Expression) },
    { new Token(t_Excl), new Nonterminal(NonterminalType::nt_Expression) },
    { new Token(t_UnMinus), new Nonterminal(NonterminalType::nt_Expression) },
    { new Token(t_Variable) },
    { new Token(t_Const) },
    { new Token(t_FuncName), new Token(t_LPar), new Nonterminal(NonterminalType::nt_Args), new Token(t_RPar) },
    { new Token(t_LPar), new Nonterminal(NonterminalType::nt_Expression), new Token(t_RPar) },
    { new Nonterminal(NonterminalType::nt_Expression), new Nonterminal(NonterminalType::nt_Args2) },
    { new Token(t_Eps) },
    { new Token(t_Comma), new Nonterminal(NonterminalType::nt_Expression), new Nonterminal(NonterminalType::nt_Args2) },
    { new Token(t_Eps) },
    // A special rule for function call - this is pushed into inputTape as temporary token for precedence parser
    { new Token(t_FuncConst) }
};


Rule Grammar4::Expand(unsigned ruleNumber)
{
    Rule toReturn = Grammar4::rightSideRules[ruleNumber - 1];
    toReturn.reverse();
    return toReturn;
}

bool Grammar4::IsRule(Rule& stackRule)
{
    // check all the possible rules for given sequence of tokens/nonterminals
    for (const auto& rule: Grammar4::rightSideRules) {
        if (rule.size() != stackRule.size()) {
            continue;
        }
        bool ruleMatch = true;
        // compare elements of grammar rule and the given sequence
        auto stackRuleMember = stackRule.cbegin();
        for (const Symbol* ruleMember: rule) {
            // if one pair is not equal, continue with next rule
            if (*ruleMember != **stackRuleMember) {
                ruleMatch = false;
                break;
            }
            stackRuleMember++;
        }
        if (ruleMatch) {
            return true;
        }
    }
    return false;
}

void Grammar4::Cleanup()
{
    for (const Rule& rule: rightSideRules) {
        for (Symbol* member: rule) {
            delete member;
        }
    }
}