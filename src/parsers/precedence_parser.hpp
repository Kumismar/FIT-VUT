/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 19:46
 */

#pragma once

#include "precedence_table.hpp"
#include "predictive_parser.hpp"
#include "token.hpp"


/**
 * @brief A class responsible for precedence parsing control.
 * @details Internally, for precedence parsing, is uses a second stack.
 * It only pops expression from the main stack once the precedence parsing is successful.
 */
class PrecedenceParser
{
private:
    /**
     * @brief The main parsing automaton pushdown.
     */
    AnalysisStack& pushdown;

    /**
     * @brief The second internal pushdown used for precedence parsing operations.
     */
    AnalysisStack analysisPushdown;

    /**
     * @brief The current token on the input tape.
     */
    Token* inputToken = nullptr;

    /**
     * @brief Precedence table.
     */
    PrecedenceTable* table = nullptr;

    /**
     * @brief Checks if precedence parsing is successful.
     * @details Precedence stack contains "$" and one expression nonterminal and input token is "$".
     */
    bool parseIsSuccessful();

    /**
     * @brief Clear the internal precedence pushdown.
     */
    void clearStack();

    /**
     * @brief After successful parsing or error, clean up allocated resources.
     * Prepare for giving control back to the main parser.
     */
    void cleanUpAfterParsing();

    /**
     * @brief Initialize precedence parsing.\
     * Push "$" to the internal pushdown and to the input tape after expression.
     */
    void initPrecedenceParsing();

    /**
     * @brief Parse the function call if token t_FuncName is on the input tape.
     */
    void parseFunction();

    /**
     * @brief Checks if there isn't token missing and if there is a function call.
     */
    void handleSpecialCases();

public:
    /**
     * @brief Precedence parser destructor.
     */
    ~PrecedenceParser();

    /**
     * @brief Precedence parser constructor.
     * 
     * @param stack The main parsing automaton pushdown.
     */
    explicit PrecedenceParser(AnalysisStack& stack);

    /**
     * @brief Parse the input tape using precedence parsing.
     */
    void Parse();
};
