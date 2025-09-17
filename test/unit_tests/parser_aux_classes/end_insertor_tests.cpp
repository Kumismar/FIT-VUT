/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 18:31
 * @ Modified time: 2024-04-29 12:43
 */

#include "ast.hpp"
#include "end_insertor.hpp"
#include "logger.hpp"
#include "token.hpp"
#include <gtest/gtest.h>

class EndInsertorTests : public ::testing::Test
{
protected:
    EndInsertor* endInsertor = nullptr;

    void SetUp() override
    {
        this->endInsertor = new EndInsertor();
        AST::GetInstance()->TurnOff();
        Logger::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        if (!inputTape.empty()) {
            for (Token* token: inputTape) {
                delete token;
            }
            inputTape.clear();
        }
        delete this->endInsertor;
        AST::Cleanup();
        Logger::Cleanup();
    }
};

TEST_F(EndInsertorTests, InsertExpressionEndSimpleExpression1)
{
    // a
    inputTape = {
        new Token(t_Const), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.begin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndSimpleExpression2)
{
    // a + b
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndSimpleExpression3)
{
    // a + b + c
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndParentheses1)
{
    // (a)
    inputTape = {
        new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndParentheses2)
{
    // (a + b)
    inputTape = {
        new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndParentheses3)
{
    // (a + b + c)
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}


TEST_F(EndInsertorTests, InsertExpressionEndNestedParentheses1)
{
    // ((a))
    inputTape = {
        new Token(t_LPar), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndNestedParentheses2)
{
    // (a + (b))
    inputTape = {
        new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndNestedParentheses3)
{
    // (a) + (b + c))
    inputTape = {
        new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_Plus), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndFunctionCall1)
{
    // f(a)
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndFunctionCall2)
{
    // f(a + b)
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndFunctionCall3)
{
    // f(a + b, f())
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeExpEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeExpEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndUnaryExpression)
{
    // -a
    inputTape = {
        new Token(t_UnMinus), new Token(t_Const), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndUnaryExpression2)
{
    // -f(a)
    inputTape = {
        new Token(t_UnMinus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndUnaryExpression3)
{
    // !(a + f(a))
    inputTape = {
        new Token(t_Excl), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndUnaryExpression4)
{
    // -f(a + b)
    inputTape = {
        new Token(t_UnMinus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndFunctionCall)
{
    // f(a + b, f(a))
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndComplexExpression1)
{
    // a + b + f(a + b, f(a))
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertExpressionEndComplexExpression2)
{
    // a + b + f(a + b, f(a)) + f(a + b, f(a))
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertExpressionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_ExpEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndSimpleFunction1)
{
    // f()
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndSimpleFunction2)
{
    // f(a)
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndSimpleFunction3)
{
    // f(a, b)
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}


TEST_F(EndInsertorTests, InsertFunctionEndNestedFunction1)
{
    // f(f())
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndNestedFunction2)
{
    // f(f(a))
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndNestedFunction3)
{
    // f(f(a), f())
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndComplexArguments1)
{
    // f(a + b, f(a, b), f(f(a)))
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_RPar), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}

TEST_F(EndInsertorTests, InsertFunctionEndComplexExpression2)
{
    // f(f() + f(a, b + g() && f(a) ((f(f(a)))), f(a)))
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_Plus), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_And), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_RPar), new Token(t_RPar), new Token(t_Comma), new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_RPar), new Token(t_RPar), new Token(t_End)
    };
    this->endInsertor->InsertFunctionEnd();
    Token* maybeFuncEnd = *(++inputTape.rbegin());
    EXPECT_EQ(maybeFuncEnd->GetTokenType(), t_FuncEnd);
}