/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:32
 */

#include "grammar_2.hpp"
#include "nonterminal.hpp"
#include "stack_item.hpp"
#include "token.hpp"

const std::vector<Rule> Grammar2::rightSideRules = {
    { new Token(TokenType::t_If), new Token(TokenType::t_LPar), new Nonterminal(NonterminalType::nt_Expression), new Token(TokenType::t_RPar), new Nonterminal(NonterminalType::nt_CodeBlock), new Nonterminal(NonterminalType::nt_If2), new Nonterminal(nStop) },
    { new Token(TokenType::t_While), new Token(TokenType::t_LPar), new Nonterminal(NonterminalType::nt_Expression), new Token(TokenType::t_RPar), new Nonterminal(NonterminalType::nt_CodeBlock), new Nonterminal(nStop) },
    { new Token(TokenType::t_For), new Token(TokenType::t_LPar), new Nonterminal(NonterminalType::nt_DeclOrExpr), new Token(TokenType::t_Semi), new Nonterminal(NonterminalType::nt_Expression), new Token(TokenType::t_Semi), new Nonterminal(NonterminalType::nt_Expression), new Token(TokenType::t_RPar), new Nonterminal(NonterminalType::nt_CodeBlock), new Nonterminal(nStop) },
    { new Nonterminal(NonterminalType::nt_DeclOrExpr), new Token(TokenType::t_Semi) },
    { new Nonterminal(NonterminalType::nt_CodeBlock) },
    { new Token(TokenType::t_Return), new Nonterminal(NonterminalType::nt_ReturnExp), new Token(TokenType::t_Semi), new Nonterminal(nStop) },
    { new Token(TokenType::t_Semi) },
    { new Token(TokenType::t_Elseif), new Token(TokenType::t_LPar), new Nonterminal(NonterminalType::nt_Expression), new Token(TokenType::t_RPar), new Nonterminal(NonterminalType::nt_CodeBlock), new Nonterminal(NonterminalType::nt_If2), new Nonterminal(nStop) },
    { new Token(TokenType::t_Else), new Nonterminal(NonterminalType::nt_CodeBlock) },
    { new Token(TokenType::t_Eps) },
    { new Nonterminal(NonterminalType::nt_Type), new Token(TokenType::t_Variable), new Token(TokenType::t_Assign), new Nonterminal(NonterminalType::nt_Expression), new Nonterminal(nStop) },
    { new Nonterminal(NonterminalType::nt_Expression) },
    { new Nonterminal(NonterminalType::nt_Expression) },
    { new Token(TokenType::t_Eps) },
};

Rule Grammar2::Expand(unsigned ruleNumber)
{
    Rule toReturn = Grammar2::rightSideRules[ruleNumber - 1];
    toReturn.reverse();
    return toReturn;
}

void Grammar2::Cleanup()
{
    for (const Rule& rule: rightSideRules) {
        for (Symbol* member: rule) {
            delete member;
        }
    }
}