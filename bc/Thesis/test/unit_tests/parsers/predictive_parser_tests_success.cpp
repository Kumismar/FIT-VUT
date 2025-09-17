/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-23 19:09
 * @ Modified time: 2024-04-29 11:00
 */

#include <gtest/gtest.h>

#include "analysis_success.hpp"
#include "ast.hpp"
#include "logger.hpp"
#include "predictive_parser.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

class PredictiveParserTestsSuccess : public ::testing::Test
{
protected:
    AnalysisStack stack;
    PredictiveParser* parser = nullptr;

    void SetUp() override
    {
        this->parser = new PredictiveParser(this->stack);
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

TEST_F(PredictiveParserTestsSuccess, VariableAssignment)
{
    inputTape = {
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfStatementEmpty)
{
    // if (const) { }
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfStatement)
{
    // if (const) { variable = const; }
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfElseStatement)
{
    // if (const) { variable = const; } else { variable = const; }
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl),
        new Token(t_Else),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfElseifStatement)
{
    // if (const) { variable = const; } elseif (const) { variable = const; }
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl),
        new Token(t_Elseif), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfElseIfElseStatement)
{
    // if (const) { variable = const; } elseif (const) { variable = const; } else { variable = const; }
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl),
        new Token(t_Elseif), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl),
        new Token(t_Else),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, IfStatementNoBraces)
{
    // if (const) variable = const;
    inputTape = {
        new Token(t_If), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, WhileLoop)
{
    // while (const) { variable = const; }
    inputTape = {
        new Token(t_While), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, WhileLoopNoBraces)
{
    // while (const) variable = const;
    inputTape = {
        new Token(t_While), new Token(t_LPar), new Token(t_Const), new Token(t_RPar),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, ForLoop)
{
    // for (variable = const; const; const) { variable = const; }
    inputTape = {
        new Token(t_For), new Token(t_LPar),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_Const), new Token(t_Semi),
        new Token(t_Const), new Token(t_RPar),
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, ForLoopNoBraces)
{
    // for (variable = const; const; const) variable = const;
    inputTape = {
        new Token(t_For), new Token(t_LPar),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_Const), new Token(t_Semi),
        new Token(t_Const), new Token(t_RPar),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, ReturnStatement)
{
    // return const;
    inputTape = {
        new Token(t_Return), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, ReturnStatementNoValue)
{
    // return;
    inputTape = {
        new Token(t_Return), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, Expression)
{
    // 3 + 4 * 5;
    inputTape = {
        new Token(t_Const), new Token(t_Plus), new Token(t_Const), new Token(t_Mul), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
}

TEST_F(PredictiveParserTestsSuccess, CodeBlock)
{
    // { variable = const; }
    inputTape = {
        new Token(t_LCurl),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, CodeBlockEmpty)
{
    // { }
    inputTape = {
        new Token(t_LCurl), new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, FunctionDefinitionNoTypeNoArgs)
{
    // function f(): { return; }
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_Colon),
        new Token(t_LCurl),
        new Token(t_Return), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, FunctionDefinitionNoTypeOneArg)
{
    // function f(int variable): { return variable; }
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar),
        new Token(t_Int), new Token(t_Variable), new Token(t_RPar), new Token(t_Colon),
        new Token(t_LCurl),
        new Token(t_Return), new Token(t_Variable), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, FunctionDefinitionNoTypeTwoArgs)
{
    // function f(int variable, int variable2): { return variable + variable2; }
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar),
        new Token(t_Int), new Token(t_Variable), new Token(t_Comma),
        new Token(t_Int), new Token(t_Variable), new Token(t_RPar), new Token(t_Colon), new Token(t_Int),
        new Token(t_LCurl),
        new Token(t_Return), new Token(t_Variable), new Token(t_Plus), new Token(t_Variable), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}

TEST_F(PredictiveParserTestsSuccess, FunctionDefinitionIntNoArgs)
{
    // function f(): int { return; }
    inputTape = {
        new Token(t_Function), new Token(t_FuncName), new Token(t_LPar), new Token(t_RPar), new Token(t_Colon),
        new Token(t_Int),
        new Token(t_LCurl),
        new Token(t_Return), new Token(t_Semi),
        new Token(t_RCurl), new Token(t_End)
    };
}

TEST_F(PredictiveParserTestsSuccess, StatementSequence)
{
    // variable = const; variable = const;
    inputTape = {
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_Variable), new Token(t_Assign), new Token(t_Const), new Token(t_Semi),
        new Token(t_End)
    };
    EXPECT_THROW(this->parser->Parse(false), SyntaxAnalysisSuccess);
}
