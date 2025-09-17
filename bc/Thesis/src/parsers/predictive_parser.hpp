/**
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 * @date 2024-03-18
 */

#pragma once

#include "grammar.hpp"
#include "ll_table.hpp"
#include "stack_item.hpp"
#include "token.hpp"

typedef std::list<Symbol*> AnalysisStack;

/**
 * @brief The main parsing class that performs predictive parsing.
 * Works as the pushdown automaton.
 * Controls the parsing algorithm.
 */
class PredictiveParser
{
private:
    /**
     * @brief The automaton pushdown. 
     */
    AnalysisStack& pushdown;

    /**
     * @brief The topmost symbol on the automaton pushdown.
     */
    Symbol* stackTop = nullptr;

    /**
     * @brief The current token on the input tape.
     */
    Token* inputToken = nullptr;

    /**
     * @brief The LL table.
     */
    LLTable* table = nullptr;

    /**
     * @brief A flag indicating the parser is currently parsing a function call.
     */
    bool parsingFunction = false;

    /**
     * @brief A flag indicating that the first function name token was already parsed.
     * The next funtionName token means giving the control to the precedence parser.
     */
    bool firstFuncName = false;

    /**
     * @brief Parse nonterminal symbol which is on top of the pushdown.
     * @details Expand the nonterminal or give control to precedence parser.
     */
    void parseNonterminal();

    /**
     * @brief Parse terminal symbol which is on top of the pushdown.
     * @details Compare the terminal with the input token and pop the terminal from the pushdown if they match.
     */
    void parseToken();

    /**
     * @brief Parse the end of the input tape.
     * @details Throw SyntaxAnalysisSuccess if both input token and topmost token on the pushdown are both "$".
     */
    void parseEnd();

    /**
     * @brief Parse the expression nonterminal.
     * @details Give control to the precedence parser.
     */
    void parseExpression(Nonterminal* nt);

    /**
     * @brief Handle special cases of the parser.
     * @details It can be either empty input tape or auxiliary token t_FuncEnd that was inserted to the input tape.
     */
    void handleSpecialCases();

public:
    /**
     * @brief Predictive parser constructor.
     * 
     * @param stack The automaton pushdown.
     */
    explicit PredictiveParser(AnalysisStack& stack);

    /**
     * @brief Predictive parser destructor.
     */
    ~PredictiveParser();

    /**
     * @brief Initialize the syntax analysis by pushing "$" on the input tape and starting nonterminal on the pushdown.
     */
    void InitSyntaxAnalysis();

    /**
     * @brief Parse the input tape using predictive parsing.
     * 
     * @param parseFunction A flag indicating the parser is currently parsing a function call.
     */
    void Parse(bool parseFunction);

    /**
     * @brief Clear the automaton pushdown after successful or unsuccessful parsing.
     */
    void ClearStack();
};