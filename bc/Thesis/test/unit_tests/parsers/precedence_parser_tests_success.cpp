/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-02 14:54
 * @ Modified time: 2024-04-29 12:36
 */

#include <gtest/gtest.h>

#include "ast.hpp"
#include "logger.hpp"
#include "nonterminal.hpp"
#include "precedence_parser.hpp"
#include "token.hpp"

class PrecedenceParserTestSuccess : public ::testing::Test
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
        AST::Cleanup();
        Logger::Cleanup();
    }
};

TEST_F(PrecedenceParserTestSuccess, SimpleAssign)
{
    inputTape = { new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
    EXPECT_TRUE(this->stack.empty());
}

TEST_F(PrecedenceParserTestSuccess, PlusOperator)
{
    inputTape = { new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MinusOperator)
{
    inputTape = { new Token(t_Variable), new Token(t_Minus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MulOperator)
{
    inputTape = { new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, DivOperator)
{
    inputTape = { new Token(t_Variable), new Token(t_Div), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ConcatOperator)
{
    inputTape = { new Token(t_Variable), new Token(t_Concat), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, AndOperator)
{
    // a && b
    inputTape = { new Token(t_Variable), new Token(t_And), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, OrOperator)
{
    // a || b
    inputTape = { new Token(t_Variable), new Token(t_Or), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}


TEST_F(PrecedenceParserTestSuccess, NotOperator)
{
    // !a
    inputTape = { new Token(t_Excl), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, UnaryMinusOperator)
{
    // -a
    inputTape = { new Token(t_UnMinus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, AddMulCombination)
{
    // a + b * c
    inputTape = { new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, SubDivCombination)
{
    // a - b / c
    inputTape = { new Token(t_Variable), new Token(t_Minus), new Token(t_Variable), new Token(t_Div), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MulAddCombination)
{
    // a * b + c
    inputTape = { new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, DivSubCombination)
{
    // a / b - c
    inputTape = { new Token(t_Variable), new Token(t_Div), new Token(t_Variable), new Token(t_Minus), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MulDivCombination)
{
    // a * b / c
    inputTape = { new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Div), new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}


TEST_F(PrecedenceParserTestSuccess, VarInParentheses)
{
    // (a)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, AddInParentheses)
{
    // (a + b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, SubInParentheses)
{
    // (a - b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Minus), new Token(t_Const), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MulInParentheses)
{
    // (a * b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, DivInParentheses)
{
    // (a / b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Div), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ConcatInParentheses)
{
    // (a . b)
    inputTape = { new Token(t_LPar), new Token(t_Variable), new Token(t_Concat), new Token(t_Const), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}


TEST_F(PrecedenceParserTestSuccess, NestedParentheses1)
{
    // (a + (b * c))
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_LPar), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
        new Token(t_RPar), new Token(t_RPar),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, NestedParentheses2)
{
    // (a + (b * c) + d)
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_LPar), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Plus), new Token(t_Variable),
        new Token(t_RPar), new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, NestedParentheses3)
{
    // (a + (b * (c + (d * e) + f) + g) + h)
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_LPar), new Token(t_Variable),
        new Token(t_Mul), new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_LPar), new Token(t_Variable),
        new Token(t_Mul), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Plus), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Plus), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Plus), new Token(t_Variable), new Token(t_RPar),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, NestedParentheses4)
{
    // ((((a + b)) * d) + (c * e))
    inputTape = {
        new Token(t_LPar), new Token(t_LPar), new Token(t_LPar), new Token(t_LPar),
        new Token(t_Variable), new Token(t_Plus), new Token(t_Variable),
        new Token(t_RPar), new Token(t_RPar),
        new Token(t_Mul), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Plus), new Token(t_LPar), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
        new Token(t_RPar), new Token(t_RPar),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}


TEST_F(PrecedenceParserTestSuccess, ParenthesesAndUnary1)
{
    // (a + (-b))
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_RPar), new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ParenthesesAndUnary2)
{
    // (-(-a))
    inputTape = {
        new Token(t_UnMinus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}


TEST_F(PrecedenceParserTestSuccess, ParenthesesUnaryBinary1)
{
    // (a + (-b) * c)
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_Mul), new Token(t_Variable),
        new Token(t_RPar), new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ParenthesesUnaryBinary2)
{
    // (-(-a) + b * c)
    inputTape = {
        new Token(t_UnMinus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ParenthesesUnaryBinary3)
{
    // (a + (-b) * c + d)
    inputTape = {
        new Token(t_LPar), new Token(t_Variable),
        new Token(t_Plus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_Mul), new Token(t_Variable),
        new Token(t_Plus), new Token(t_Variable),
        new Token(t_RPar), new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ParenthesesUnaryBinary4)
{
    // (-(-a) + b * c + d)
    inputTape = {
        new Token(t_UnMinus), new Token(t_UnMinus), new Token(t_Variable),
        new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
        new Token(t_Plus), new Token(t_Variable),
        new Token(t_ExpEnd)
    };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNoArgs)
{
    // f()
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallOneArg)
{
    // f(a)
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallTwoArgs)
{
    // f(a, b)
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallThreeArgs)
{
    // f(a, b, c)
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested)
{
    // f(a, g(b, c))
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested2)
{
    // f(g(a, b), c)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested3)
{
    // f(g(a, b), h(c, d))
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested4)
{
    // f(g(a, h(b, c)), d)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested5)
{
    // f(g(a, h(b, c)), i(d, e))
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallNested6)
{
    // f(g(a, h(b, c)), i(d, e, f))
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ExpressionInFunctionCall)
{
    // f(a + b, c * d)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ExpressionInFunctionCall2)
{
    // f(a + b * c, d)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ExpressionInFunctionCall3)
{
    // f(a + b * c, d + e)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ExpressionInFunctionCall4)
{
    // f(a + b * c, d + e * f)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, ExpressionInFunctionCall5)
{
    // f(a + b * c, d + e * f + g)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallsInExpression)
{
    // a + f(b, c)
    inputTape = { new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallsInExpression2)
{
    // a + f(b, c) * d
    inputTape = { new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Mul),
                  new Token(t_Variable), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallsInExpression3)
{
    // a + f(b, c) * d + e
    inputTape = { new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Mul),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable),
                  new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallsInExpression4)
{
    // a + f(b, c) * d + e * f(g, h)
    inputTape = { new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Mul),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar),
                  new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, FunctionCallsInExpression5)
{
    // a + f(b, c) * d + e * f(g, h) + i(j, k)
    inputTape = { new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Mul),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Mul),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar),
                  new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, MixedFunctionCallsAndExpressions)
{
    // f(a + b, c * d) + e * f(g, h) + i(j, k)
    inputTape = { new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Comma),
                  new Token(t_Variable), new Token(t_Mul), new Token(t_Variable),
                  new Token(t_RPar), new Token(t_Plus),
                  new Token(t_Variable), new Token(t_Mul),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable), new Token(t_RPar),
                  new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, NestedFunctionCallsAndExpressions)
{
    // f(a + f(b(c + 1), c(g(3) * 5)), d * f(g, h))
    inputTape = { new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Plus),
                  new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Const),
                  new Token(t_RPar), new Token(t_Comma),
                  new Token(t_FuncName), new Token(t_LPar),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Const),
                  new Token(t_RPar), new Token(t_Mul), new Token(t_Const),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_Comma), new Token(t_Variable), new Token(t_Mul),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Comma), new Token(t_Variable),
                  new Token(t_RPar),
                  new Token(t_RPar), new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}

TEST_F(PrecedenceParserTestSuccess, AssignComplexExpression)
{
    // a = ((b = f(i + 2)) != 0)
    inputTape = { new Token(t_Variable), new Token(t_Assign), new Token(t_LPar),
                  new Token(t_LPar), new Token(t_Variable), new Token(t_Assign),
                  new Token(t_FuncName), new Token(t_LPar), new Token(t_Variable), new Token(t_Plus), new Token(t_Const), new Token(t_RPar),
                  new Token(t_RPar), new Token(t_NEq), new Token(t_Const),
                  new Token(t_RPar),
                  new Token(t_ExpEnd) };
    EXPECT_NO_THROW(this->parser->Parse());
}
