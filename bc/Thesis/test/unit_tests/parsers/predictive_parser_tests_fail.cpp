/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-02 12:42
 * @ Modified time: 2024-04-29 12:36
 */

#include <gtest/gtest.h>

#include "ast.hpp"
#include "logger.hpp"
#include "predictive_parser.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

class PredictiveParserTestsFail : public ::testing::Test
{
protected:
    AnalysisStack stack;
    PredictiveParser* parser = nullptr;

    void SetUp() override
    {
        this->parser = new PredictiveParser(stack);
        this->parser->InitSyntaxAnalysis();
        AST::GetInstance()->TurnOff();
        Logger::GetInstance()->TurnOff();
    }

    void TearDown() override
    {
        this->parser->ClearStack();
        for (Token* token: inputTape) {
            delete token;
        }
        inputTape.clear();
        delete this->parser;
        AST::Cleanup();
        Logger::Cleanup();
    }
};

TEST_F(PredictiveParserTestsFail, EmptyInput)
{
    inputTape = { new Token(t_End) };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, MissingEndToken)
{
    inputTape = { new Token(t_Const), new Token(t_Semi) };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionCallMissingClosingParenthesis)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Semi), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionCallMissingOpeningParenthesis)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_RPar), new Token(t_Semi), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionCallMissingComma)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Const), new Token(t_RPar), new Token(t_Semi), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, MissingSemicolon)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, MissingSemicolonTwoStatements)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_RPar), new Token(t_Semi),
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Const), new Token(t_Comma), new Token(t_Const), new Token(t_RPar), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, ForLoopMissingInitialization)
{
    inputTape = {
        new Token(t_For), new Token(t_LPar), new Token(t_Semi), new Token(t_Const), new Token(t_Semi), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, ForLoopMissingCondition)
{
    inputTape = {
        new Token(t_For), new Token(t_LPar), new Token(t_Const), new Token(t_Semi), new Token(t_Semi), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, ForLoopMissingIncrement)
{
    inputTape = {
        new Token(t_For), new Token(t_LPar), new Token(t_Const), new Token(t_Semi), new Token(t_Const), new Token(t_Semi), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, WhileLoopMissingCondition)
{
    inputTape = {
        new Token(t_While), new Token(t_LPar), new Token(t_RPar), new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, IfStatementMissingCondition)
{
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_RPar), new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, IfStatementNonsenseCondition)
{
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Return), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionDefinitionMissingComma)
{
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar), new Token(t_Int), new Token(t_Variable), new Token(t_Int), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Colon),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionDefinitionMissingColon)
{
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar), new Token(t_Int), new Token(t_Variable), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}

TEST_F(PredictiveParserTestsFail, FunctionDefinitionNoFunctionKeyword)
{
    inputTape = {
        new Token(t_FuncName), new Token(t_LPar), new Token(t_Int), new Token(t_Variable), new Token(t_RPar),
        new Token(t_Colon),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxError);
}