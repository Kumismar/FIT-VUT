/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 18:31
 * @ Modified time: 2024-04-30 13:58
 */

#include "ast.hpp"
#include "common.hpp"
#include "internal_error.hpp"
#include "logger.hpp"
#include "nonterminal.hpp"
#include "pattern_finder.hpp"
#include "precedence_symbol.hpp"
#include "token.hpp"
#include <gtest/gtest.h>

class PatternFinderTests : public ::testing::Test
{
protected:
    AnalysisStack stack;
    PatternFinder* finder = nullptr;
    Rule rule;
    Rule expected;

    void SetUp() override
    {
        finder = new PatternFinder(this->stack);
        AST::GetInstance()->TurnOff();
        Logger::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        delete finder;
        if (!this->stack.empty()) {
            for (Symbol* symb: this->stack) {
                delete symb;
            }
            this->stack.clear();
        }
        AST::Cleanup();
        Logger::Cleanup();
        cleanRule(this->rule);
        cleanRule(this->expected);
    }
};


TEST_F(PatternFinderTests, FindFirstTokenInStackOneToken)
{
    this->stack = {
        new Token(t_Const),
    };
    Token* token = finder->FindFirstToken();
    EXPECT_EQ(token->GetTokenType(), t_Const);
}

TEST_F(PatternFinderTests, FindFirstTokenInStackMultipleTokens)
{
    this->stack = {
        new Token(t_Const),
        new Token(t_Variable),
        new Token(t_ExpEnd),
    };
    Token* token = finder->FindFirstToken();
    EXPECT_EQ(token->GetTokenType(), t_Const);
}

TEST_F(PatternFinderTests, FindFirstTokenInStackNoToken)
{
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression)
    };
    EXPECT_THROW(finder->FindFirstToken(), InternalError);
}

TEST_F(PatternFinderTests, FindFirstTokenInStackTokenBetweenNonterminals)
{
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Const),
        new Nonterminal(NonterminalType::nt_Expression)
    };
    Token* token = finder->FindFirstToken();
    EXPECT_EQ(token->GetTokenType(), t_Const);
}

TEST_F(PatternFinderTests, FindFirstRuleOneConstant)
{
    this->stack = {
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Token(t_Const),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}

TEST_F(PatternFinderTests, FindFirstRuleMultipleConstants)
{
    this->stack = {
        new Token(t_Const),
        new Token(t_Variable),
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Token(t_Const),
        new Token(t_Variable),
        new Token(t_Const),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}

TEST_F(PatternFinderTests, FindFirstRuleNonterminal)
{
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}

TEST_F(PatternFinderTests, FindFirstRuleMultipleNonterminals)
{
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression),
        new Nonterminal(NonterminalType::nt_Expression),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}

TEST_F(PatternFinderTests, FindFirstRuleMixed)
{
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Const),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Variable),
        new Nonterminal(NonterminalType::nt_Expression),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Const),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Variable),
        new Nonterminal(NonterminalType::nt_Expression),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}

TEST_F(PatternFinderTests, FindFirstRuleMixed2)
{
    this->stack = {
        new Token(t_Const),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Variable),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd),
    };

    finder->FindFirstRule(this->rule);
    this->expected = {
        new Token(t_Const),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Variable),
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Const),
    };
    this->expected.reverse();
    EXPECT_TRUE(handlesEqual(rule, expected));
}
