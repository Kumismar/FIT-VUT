/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 21:48
 * @ Modified time: 2024-04-29 12:48
 */

#include "ast.hpp"
#include "common.hpp"
#include "expression_processor.hpp"
#include "logger.hpp"
#include "nonterminal.hpp"
#include "precedence_symbol.hpp"
#include "token.hpp"
#include <gtest/gtest.h>


class ExpressionProcessorTests : public ::testing::Test
{
protected:
    ExpressionProcessor* processor = nullptr;
    AnalysisStack stack;
    Rule expected;

    void SetUp() override
    {
        this->processor = new ExpressionProcessor(this->stack);
        this->stack = {
            new Token(t_ExpEnd)
        };
        Logger::GetInstance()->TurnOff();
        AST::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        if (!inputTape.empty()) {
            for (Token* token: inputTape) {
                delete token;
            }
            inputTape.clear();
        }

        cleanRule(this->stack);
        cleanRule(this->expected);

        AST::Cleanup();
        Logger::Cleanup();
        delete this->processor;
    }
};

TEST_F(ExpressionProcessorTests, ShiftTest1)
{
    inputTape = {
        new Token(t_Const),
    };
    this->processor->Shift();
    this->expected = {
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ShiftTest2)
{
    inputTape = {
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
    };
    this->processor->Shift();
    this->expected = {
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ShiftTest3)
{
    inputTape = {
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
    };
    this->processor->Shift();
    this->expected = {
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, PushTest1)
{
    inputTape = {
        new Token(t_Plus),
        new Token(t_Const),
    };
    this->processor->Push();
    this->expected = {
        new Token(t_Plus),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, PushTest2)
{
    inputTape = {
        new Token(t_Variable),
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
    };
    this->processor->Push();
    this->expected = {
        new Token(t_Variable),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, PushTest3)
{
    inputTape = {
        new Token(t_Const),
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
        new Token(t_Plus),
        new Token(t_Const),
    };
    this->processor->Push();
    this->expected = {
        new Token(t_Const),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ReduceTest1)
{
    Symbol* tmp = this->stack.front();
    this->stack.pop_front();
    this->stack = {
        new Token(t_Const),
        new PrecedenceSymbol(PrecedenceType::Push),
    };
    this->stack.push_back(tmp);
    this->processor->Reduce();
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ReduceTest2)
{
    Symbol* tmp = this->stack.front();
    this->stack.pop_front();
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_Plus),
        new Nonterminal(NonterminalType::nt_Expression),
        new PrecedenceSymbol(PrecedenceType::Push),
    };
    this->stack.push_back(tmp);
    this->processor->Reduce();
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ReduceTest3)
{
    Symbol* tmp = this->stack.front();
    this->stack.pop_front();
    this->stack = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_UnMinus),
        new PrecedenceSymbol(PrecedenceType::Push),
    };
    this->stack.push_back(tmp);

    this->processor->Reduce();
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}

TEST_F(ExpressionProcessorTests, ReduceTest4)
{
    Symbol* tmp = this->stack.front();
    this->stack.pop_front();
    this->stack = {
        new Token(t_FuncConst),
        new PrecedenceSymbol(PrecedenceType::Push),
    };
    this->stack.push_back(tmp);
    this->processor->Reduce();
    this->expected = {
        new Nonterminal(NonterminalType::nt_Expression),
        new Token(t_ExpEnd)
    };
    EXPECT_TRUE(handlesEqual(this->expected, this->stack));
}