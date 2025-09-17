/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-07 14:28
 * @ Modified time: 2024-04-29 11:53
 */

#include "ast_node_factory.hpp"
#include "ast.hpp"
#include "ast_node.hpp"
#include "binary_expression.hpp"
#include "constant.hpp"
#include "declaration.hpp"
#include "elseif_statement.hpp"
#include "for_loop.hpp"
#include "function_definition.hpp"
#include "if_statement.hpp"
#include "internal_error.hpp"
#include "nonterminal.hpp"
#include "return_statement.hpp"
#include "statement_list.hpp"
#include "token.hpp"
#include "unary_expression.hpp"
#include "variable.hpp"
#include "while_loop.hpp"

ASTNode* ASTNodeFactory::CreateASTNode(Nonterminal& nt, Token& t)
{
    if (AST::GetInstance()->IsTurnedOff()) {
        return nullptr;
    }

    switch (nt.GetNonterminalType()) {
        case NonterminalType::nt_Statement: {
            switch (t.GetTokenType()) {
                case TokenType::t_If: {
                    return new IfStatement();
                }
                case TokenType::t_While: {
                    return new WhileLoop();
                }
                case TokenType::t_For: {
                    return new ForLoop();
                }
                case TokenType::t_Return: {
                    return new ReturnStatement();
                }
                default: {
                    return nullptr;
                }
            }
        }
        case NonterminalType::nt_DeclOrExpr: {
            switch (t.GetTokenType()) {
                case TokenType::t_Int:
                case TokenType::t_Float:
                case TokenType::t_String:
                case TokenType::t_Bool: {
                    return new Declaration();
                }
                default: {
                    return nullptr;
                }
            }
        }
        case NonterminalType::nt_If2: {
            if (t.GetTokenType() == TokenType::t_Elseif) {
                return new ElseifStatement();
            }
            return nullptr;
        }
        case NonterminalType::nt_FunctionDef: {
            return new FunctionDefinition();
        }
        case NonterminalType::nt_CodeBlock: {
            return new StatementList();
        }
        case NonterminalType::nt_Statements: {
            if (AST::GetInstance()->GetCurrentContext()->GetNodeType() == NodeType::nodeFuncDef) {
                return new StatementList();
            }
            return nullptr;
        }
        default: {
            return nullptr;
        }
    }
}
Expression* ASTNodeFactory::CreateASTNode(Rule& rule)
{
    //right side is either variable, const or funcConst
    if (AST::GetInstance()->IsTurnedOff()) {
        return nullptr;
    }

    if (rule.size() == 1) {
        return ASTNodeFactory::createOperand(rule);
    }
    else if (ASTNodeFactory::isUnaryExpression(rule)) {
        return ASTNodeFactory::createUnaryExpression(rule);
    }
    else if (rule.size() == 3) {
        return ASTNodeFactory::createBinaryExpression(rule);
    }
    else {
        throw InternalError("ASTNodeFactory:: rule size != {1, 2, 3}\n");
    }
}

bool ASTNodeFactory::isUnaryExpression(Rule& rule)
{
    if (rule.size() == 2) {
        return true;
    }

    auto* tmp = dynamic_cast<Token*>(rule.front());
    if (tmp != nullptr) {
        return (*tmp == TokenType::t_LPar);
    }
    return false;
}

Expression* ASTNodeFactory::createOperand(Rule& rule)
{
    Symbol* tmpSymb = rule.front();
    auto* operandToken = dynamic_cast<Token*>(tmpSymb);
    if (operandToken == nullptr) {
        throw InternalError("ASTNodeFactory::createOperand operand is not Token*, real type:" + std::string(typeid(*tmpSymb).name()) + "\n");
    }

    if (*operandToken == TokenType::t_Variable) {
        auto* tmpVar = new Variable(*operandToken);
        return tmpVar;
    }
    else if (*operandToken == TokenType::t_Const) {
        auto* tmpConst = new Constant(*operandToken);
        return tmpConst;
    }
    else if (*operandToken == TokenType::t_FuncConst) {
        // Function call node was already created, just return
        return nullptr;
    }
    else {
        throw InternalError("ASTNodeFactory::createOperand token is not var or const\n");
    }
}

Expression* ASTNodeFactory::createUnaryExpression(Rule& rule)
{
    Symbol* tmpSymb = *rule.begin();
    auto* optr = dynamic_cast<Token*>(tmpSymb);
    if (optr == nullptr) {
        throw InternalError("ASTNodeFactory::createUnaryExpression optr is not Token*, real type:" + std::string(typeid(*tmpSymb).name()) + "\n");
    }

    Expression* tmpExp = new UnaryExpression(optr->GetTokenType());
    return tmpExp;
}

Expression* ASTNodeFactory::createBinaryExpression(Rule& rule)
{
    Token* possibleLPar = dynamic_cast<Token*>(*rule.begin());
    if (possibleLPar != nullptr && *possibleLPar == TokenType::t_LPar) {
        auto* tmpExp = new UnaryExpression(TokenType::t_LPar);
        return tmpExp;
    }

    Symbol* tmpSymb = *std::next(rule.begin());
    auto* optr = dynamic_cast<Token*>(tmpSymb);
    if (optr == nullptr) {
        throw InternalError("ASTNodeFactory::createBinaryExpression optr is not Token*, real type:" + std::string(typeid(*tmpSymb).name()) + "\n");
    }

    auto* tmpExp = new BinaryExpression(optr->GetTokenType());
    return tmpExp;
}
