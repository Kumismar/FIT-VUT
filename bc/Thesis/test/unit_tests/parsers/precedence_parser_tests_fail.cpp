/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-03 17:28
 * @ Modified time: 2024-05-01 12:03
 */

#include <gtest/gtest.h>

#include "ast.hpp"
#include "logger.hpp"
#include "nonterminal.hpp"
#include "precedence_parser.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

class PrecedenceParserTestsFail : public ::testing::Test
{
protected:
    AnalysisStack stack;
    PrecedenceParser* parser = nullptr;

    void SetUp() override
    {
        this->parser = new PrecedenceParser(this->stack);
        this->stack.push_front(new Nonterminal(NonterminalType::nt_Expression));
        AST::GetInstance()->TurnOff();
        Logger::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        for (Token* token: inputTape) {
            delete token;
        }
        inputTape.clear();
        delete this->parser;

        if (!this->stack.empty()) {
            for (Symbol* symb: this->stack) {
                delete symb;
            }
            this->stack.clear();
        }

        AST::Cleanup();
        Logger::Cleanup();
    }
};

TEST_F(PrecedenceParserTestsFail, EmptyInput)
{
    inputTape = { new Token(t_End) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, UnexpectedTokenInExpression)
{
    // a + ; b
    inputTape = { new Token(t_Const), new Token(t_Plus), new Token(t_Semi), new Token(t_Const), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, InvalidFunctionCallSyntax)
{
    // f(a + b,)
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Comma), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, InvalidExpressionWithinParentheses)
{
    // (a + b +)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Plus), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, ConsecutiveOperators)
{
    // a + + b
    inputTape = { new Token(t_Variable), new Token(t_Plus), new Token(t_Plus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, MissingOperand)
{
    // a +
    inputTape = { new Token(t_Variable), new Token(t_Plus), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, InvalidUnaryOperator)
{
    // * a
    inputTape = { new Token(t_Mul), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, UnmatchedParentheses)
{
    // (a + b
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}

TEST_F(PrecedenceParserTestsFail, MissingOperatorInParentheses)
{
    // (a b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_THROW(this->parser->Parse(), SyntaxError);
}
