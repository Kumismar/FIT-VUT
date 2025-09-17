/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-05-01 11:33
 * @ Modified time: 2024-05-01 13:26
 */

#include "ast.hpp"
#include "ast_node_factory.hpp"
#include "binary_expression.hpp"
#include "common.hpp"
#include "expression.hpp"
#include "logger.hpp"
#include "token.hpp"
#include "unary_expression.hpp"
#include <gtest/gtest.h>
#include <vector>


class ASTLinkingTests : public ::testing::Test
{
protected:
    ASTNodeFactory* factory = nullptr;
    AST* ast = nullptr;
    Rule rule;
    std::vector<Token*> tokens;

    void SetUp() override
    {
        this->factory = new ASTNodeFactory();
        this->ast = AST::GetInstance();

        Logger::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        delete this->factory;
        AST::Cleanup();
        Logger::Cleanup();

        if (!this->tokens.empty()) {
            for (Token* t: this->tokens) {
                delete t;
            }
        }
    }
};

TEST_F(ASTLinkingTests, LinkUnaryExpression)
{
    this->tokens = {
        sampleToken()
    };
    createOperandAndPush(this->tokens[0]);
    Expression* operand = this->ast->GetExpressionContext();

    Expression* context = createUnaryExpression(t_Plus);
    auto* uExpr = dynamic_cast<UnaryExpression*>(context);

    ASSERT_NE(uExpr, nullptr);
    EXPECT_EQ(uExpr->GetOperand(), operand);
}

TEST_F(ASTLinkingTests, LinkBinaryExpression)
{
    this->tokens = {
        sampleToken(),
        sampleToken()
    };

    createOperandAndPush(this->tokens[0]);
    Expression* operand1 = this->ast->GetExpressionContext();
    createOperandAndPush(this->tokens[1]);
    Expression* operand2 = this->ast->GetExpressionContext();

    Expression* context = createBinaryExpression(t_Plus);
    auto* bExpr = dynamic_cast<BinaryExpression*>(context);

    ASSERT_NE(bExpr, nullptr);
    EXPECT_EQ(bExpr->GetLeft(), operand1);
    EXPECT_EQ(bExpr->GetRight(), operand2);
}