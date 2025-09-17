/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:33
 */

#include "grammar_3.hpp"
#include "nonterminal.hpp"
#include "stack_item.hpp"
#include "token.hpp"

const std::vector<Rule> Grammar3::rightSideRules = {
    { new Token(t_Function), new Token(t_FuncName), new Token(t_LPar), new Nonterminal(NonterminalType::nt_Params), new Token(t_RPar), new Token(t_Colon), new Nonterminal(NonterminalType::nt_VoluntaryType), new Token(t_LCurl), new Nonterminal(NonterminalType::nt_Statements), new Nonterminal(nStop), new Token(t_RCurl), new Nonterminal(nStop) },
    { new Nonterminal(NonterminalType::nt_Type), new Token(t_Variable), new Nonterminal(NonterminalType::nt_Params2) },
    { new Token(t_Eps) },
    { new Token(t_Comma), new Nonterminal(NonterminalType::nt_Type), new Token(t_Variable), new Nonterminal(NonterminalType::nt_Params2) },
    { new Token(t_Eps) },
};


Rule Grammar3::Expand(unsigned ruleNumber)
{
    Rule toReturn = Grammar3::rightSideRules[ruleNumber - 1];
    toReturn.reverse();
    return toReturn;
}

void Grammar3::Cleanup()
{
    for (const Rule& rule: rightSideRules) {
        for (Symbol* member: rule) {
            delete member;
        }
    }
}