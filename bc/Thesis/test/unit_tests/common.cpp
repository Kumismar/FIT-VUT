/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-29 09:16
 * @ Modified time: 2024-05-02 22:08
 */


#include "common.hpp"
#include "ast.hpp"
#include "ast_node_factory.hpp"
#include "binary_expression.hpp"
#include "grammar.hpp"
#include "unary_expression.hpp"
#include <cassert>
#include <vector>

bool handlesEqual(Rule& r1, Rule& r2)
{
    if (r1.size() != r2.size()) {
        return false;
    }

    auto it1 = r1.begin();
    auto it2 = r2.begin();
    for (; it1 != r1.end(); ++it1, ++it2) {
        if (**it1 != **it2) {
            return false;
        }
    }
    return true;
}

void cleanRule(Rule& rule)
{
    for (Symbol* symb: rule) {
        delete symb;
    }
    rule.clear();
}

Token* sampleToken(TokenType type, DataType dType, Value value)
{
    auto* token = new Token(type);
    token->SetData(dType, value);
    return token;
}

void createOperandAndPush(Token* t)
{
    Rule rule = {
        t
    };
    ASTNodeFactory f;
    Expression* expr = f.CreateASTNode(rule);
    assert(expr != nullptr);

    AST::GetInstance()->PushExpressionContext(expr);
}

Expression* createUnaryExpression(TokenType type)
{
    auto* expr = new UnaryExpression(type);
    AST::GetInstance()->PushExpressionContext(expr);
    return AST::GetInstance()->GetExpressionContext();
}

Expression* createBinaryExpression(TokenType type)
{
    auto* expr = new BinaryExpression(type);
    AST::GetInstance()->PushExpressionContext(expr);
    return AST::GetInstance()->GetExpressionContext();
}
