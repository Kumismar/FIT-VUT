/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:33
 */

#include "grammar_5.hpp"
#include "nonterminal.hpp"
#include "stack_item.hpp"
#include "token.hpp"

const std::vector<Rule> Grammar5::rightSideRules = {
    { new Token(t_LCurl), new Nonterminal(NonterminalType::nt_Statements), new Token(t_RCurl), new Nonterminal(nStop) },
    { new Nonterminal(NonterminalType::nt_Statement), new Nonterminal(nStop) },
    { new Nonterminal(NonterminalType::nt_Statement), new Nonterminal(NonterminalType::nt_Statements) },
    { new Token(t_Eps) }
};

Rule Grammar5::Expand(unsigned ruleNumber)
{
    Rule toReturn = Grammar5::rightSideRules[ruleNumber - 1];
    toReturn.reverse();
    return toReturn;
}

void Grammar5::Cleanup()
{
    for (const Rule& rule: rightSideRules) {
        for (Symbol* member: rule) {
            delete member;
        }
    }
}