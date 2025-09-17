/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-28 18:35
 * @ Modified time: 2024-05-01 13:27
 */

#include "ast.hpp"
#include "ast_node_factory.hpp"
#include "binary_expression.hpp"
#include "common.hpp"
#include "constant.hpp"
#include "logger.hpp"
#include "token.hpp"
#include "unary_expression.hpp"
#include "variable.hpp"
#include <gtest/gtest.h>

class ASTNodeFactoryTests : public ::testing::Test
{
protected:
    Rule rule;
    ASTNodeFactory* factory = nullptr;
    Expression* expr = nullptr;

    void SetUp() override
    {
        this->factory = new ASTNodeFactory();
        Logger::GetInstance()->TurnOff();
        this->expr = nullptr;
    }

    void TearDown() override
    {
        delete this->factory;

        if (!this->rule.empty()) {
            for (Symbol* symb: this->rule) {
                delete symb;
            }
        }
        AST::Cleanup();
        Logger::Cleanup();

        delete this->expr;
    }
};

TEST_F(ASTNodeFactoryTests, CreateConstant)
{
    this->rule = {
        sampleToken()
    };
    this->expr = this->factory->CreateASTNode(this->rule);
    EXPECT_EQ(typeid(Constant), typeid(*expr));
}

TEST_F(ASTNodeFactoryTests, CreateVariable)
{
    this->rule = {
        sampleToken(t_Variable)
    };
    this->expr = this->factory->CreateASTNode(this->rule);
    EXPECT_EQ(typeid(Variable), typeid(*expr));
}

TEST_F(ASTNodeFactoryTests, CreateUnaryExpression)
{
    Token* constant = sampleToken(t_Const);
    createOperandAndPush(constant);

    Token* unMinus = sampleToken(t_UnMinus);
    this->rule = {
        unMinus,
        constant
    };
    this->expr = this->factory->CreateASTNode(this->rule);
    EXPECT_EQ(typeid(UnaryExpression), typeid(*expr));
}

TEST_F(ASTNodeFactoryTests, CreateBinaryExpression)
{
    Token* constant1 = sampleToken(t_Const);
    createOperandAndPush(constant1);

    Token* constant2 = sampleToken(t_Const);
    createOperandAndPush(constant2);

    Token* plus = sampleToken(t_Plus);
    this->rule = {
        constant1,
        plus,
        constant2
    };
    this->expr = this->factory->CreateASTNode(this->rule);
    EXPECT_EQ(typeid(BinaryExpression), typeid(*expr));
}