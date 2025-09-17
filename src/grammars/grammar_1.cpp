/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:32
 */

#include "grammar_1.hpp"
#include "nonterminal.hpp"
#include "stack_item.hpp"
#include "token.hpp"

const std::vector<Rule> Grammar1::rightSideRules = {
    { new Nonterminal(NonterminalType::nt_Statement), new Nonterminal(NonterminalType::nt_StatementList) },
    { new Nonterminal(NonterminalType::nt_FunctionDef), new Nonterminal(NonterminalType::nt_StatementList) },
    { new Nonterminal(NonterminalType::nt_Statement), new Nonterminal(NonterminalType::nt_StatementList) },
    { new Nonterminal(NonterminalType::nt_FunctionDef), new Nonterminal(NonterminalType::nt_StatementList) },
    { new Token(TokenType::t_Eps) }
};

Rule Grammar1::Expand(unsigned ruleNumber)
{
    Rule toReturn = Grammar1::rightSideRules[ruleNumber - 1];
    toReturn.reverse();
    return toReturn;
}

void Grammar1::Cleanup()
{
    for (const Rule& rule: rightSideRules) {
        for (Symbol* member: rule) {
            delete member;
        }
    }
}