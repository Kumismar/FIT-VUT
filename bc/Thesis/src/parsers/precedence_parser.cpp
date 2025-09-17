/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-04-30 13:54
 */

#include "precedence_parser.hpp"
#include "ast.hpp"
#include "end_insertor.hpp"
#include "expression_processor.hpp"
#include "function_call.hpp"
#include "function_parsed.hpp"
#include "internal_error.hpp"
#include "nonterminal.hpp"
#include "pattern_finder.hpp"
#include "predictive_parser.hpp"
#include "stack_item.hpp"
#include "syntax_error.hpp"
#include "token.hpp"

PrecedenceParser::PrecedenceParser(AnalysisStack& stack)
    : pushdown(stack)
{
    this->table = new PrecedenceTable();
}

PrecedenceParser::~PrecedenceParser()
{
    delete this->table;

    this->clearStack(); // affects only in unit tests
}

void PrecedenceParser::Parse()
{
    this->initPrecedenceParsing();

    while (true) {
        this->inputToken = inputTape.front();
        this->handleSpecialCases();

        if (this->parseIsSuccessful()) {
            this->cleanUpAfterParsing();
            break; // while
        }

        PatternFinder finder(this->analysisPushdown);
        Token* firstToken = finder.FindFirstToken();

        ExpressionProcessor handler(this->analysisPushdown);
        switch ((*this->table)[*firstToken][*this->inputToken]) {
            case '=': {
                handler.Push();
                break; // switch
            }
            case '<': {
                handler.Shift();
                break; // switch
            }
            case '>': {
                try {
                    handler.Reduce();
                }
                catch (SyntaxError const& e) {
                    this->clearStack();
                    throw;
                }
                break; // switch
            }
            case 'x': {
                this->clearStack();
                throw SyntaxError("Invalid token.\n");
            }
            default: {
                this->clearStack();
                throw InternalError("Something else than '<', '=', '>', 'x' in precedence table.\n");
            }
        }
    }
}

bool PrecedenceParser::parseIsSuccessful()
{
    // Only $E left on stack
    if (this->analysisPushdown.size() != 2) {
        return false;
    }

    Symbol* top = this->analysisPushdown.front();
    this->analysisPushdown.pop_front();
    Symbol* second = this->analysisPushdown.front();
    this->analysisPushdown.push_front(top);

    return (*this->inputToken == TokenType::t_ExpEnd && *second == Token(TokenType::t_ExpEnd) && *top == Nonterminal(NonterminalType::nt_Expression));
}

void PrecedenceParser::clearStack()
{
    while (!this->analysisPushdown.empty()) {
        delete this->analysisPushdown.front();
        this->analysisPushdown.pop_front();
    }
}

void PrecedenceParser::cleanUpAfterParsing()
{
    this->clearStack();
    delete this->inputToken;
    inputTape.pop_front();
    delete this->pushdown.front();
    this->pushdown.pop_front();
}

void PrecedenceParser::initPrecedenceParsing()
{
    EndInsertor endInsertor;

    this->analysisPushdown.push_front(new Token(t_ExpEnd));
    endInsertor.InsertExpressionEnd();
}

void PrecedenceParser::parseFunction()
{
    EndInsertor endInsertor;
    PredictiveParser predParser(this->pushdown);

    try {
        endInsertor.InsertFunctionEnd();
        if (!AST::GetInstance()->IsTurnedOff()) {
            auto* tmp = new FunctionCall();
            AST::GetInstance()->PushContext(tmp);
        }
        predParser.Parse(true);
    }
    catch (FunctionParsed const& e) {
        ASTNode* context = AST::GetInstance()->GetCurrentContext();
        if (context != nullptr) { // unit tests only
            auto* parsedFuncCall = dynamic_cast<FunctionCall*>(context);
            if (parsedFuncCall == nullptr) {
                throw InternalError("PrecedenceParser::parseFunction current context isnt fCall after fCall parsing, real type:" + std::string(typeid(*context).name()));
            }
            AST::GetInstance()->PushExpressionContext(parsedFuncCall);
            AST::GetInstance()->PopContext();
        }
        this->inputToken = inputTape.front();
    }
}

void PrecedenceParser::handleSpecialCases()
{
    if (inputTape.empty() || this->inputToken == nullptr) {
        this->clearStack();
        throw SyntaxError("Missing token(s).\n");
    }

    if (*this->inputToken == t_FuncName) {
        this->parseFunction();
    }
}