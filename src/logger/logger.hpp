/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:03
 */

#pragma once

#include "grammar.hpp"
#include "stack_item.hpp"
#include "token.hpp"
#include <fstream>

/**
 * @brief Auxiliary class for logging any errors or debug information.
 */
class Logger
{
private:
    /**
     * @brief ANSI escape code for red.
     */
    std::string red = "\033[1;31m";

    /**
     * @brief ANSI escape code for resetting text to default look.
     */
    std::string reset = "\033[0m";

    /**
     * @brief ANSI escape code for underlining text.
     */
    std::string underlined = "\033[4m";

    /**
     * @brief A singleton instance of the logger.
     */
    static Logger* instance;

    /**
     * @brief A list of tokens that were recently parsed.
     */
    std::list<Token*> recentTokens;

    /**
     * @brief Output file stream.
     */
    std::ofstream file;

    /**
     * @brief A left side of rule that will be logged to sequence of rules used for parsing to output.log.
     */
    Symbol* leftSideRule = nullptr;

    /**
     * @brief A right side of rule that will be logged to sequence of rules used for parsing to output.log.
     */
    Rule rightSideRule;

    /**
     * @brief Enables printing auxiliary debug information, which was formerly used for development.
     */
    bool enableDebugPrint = false;

    /**
     * @brief Disables logger.
     */
    bool turnedOff = false;

    /**
     * @brief Private constructor of the logger.
     * Is called from GetInstance() method.
     */
    Logger();

    /**
     * @brief Private destructor of the logger.
     * Is called from Cleanup() method.
     */
    ~Logger();

    /**
     * @brief Clears left and right side of rule after logging to output.log.
     */
    void clearRule();

public:
    /**
    * @brief Creates an instance of the logger if it does not exist.
    * @return An instance of the logger.
    */
    static Logger* GetInstance();

    /**
     * @brief Deletes an instance of the logger it it exists.
     */
    static void Cleanup();

    /**
     * @brief Prepares left side of the most recent rule that was used for parsing for printing.
     * @param leftSide A left side of the rule.
     */
    void AddLeftSide(Symbol* leftSide);

    /**
     * @brief Prepares right side of the most recent rule that was used for parsing for printing.
     * @param rightSide A right side of the rule.
     */
    void AddRightSide(Rule& rightSide);

    /**
     * @brief Prints the most recent rule that was used for parsing to output.log.
     * AddRightSide() and AddLeftSide() must be called before this method.
     */
    void PrintRule();

    /**
     * @brief Prints all tokens in input tape.
     * Used for development purposes.
     */
    void PrintInputTape() const;

    /**
     * @brief Adds token to the list of recently parsed tokens.
     * @param token A token to be added.
     */
    void AddTokenToRecents(Token& token);

    /**
     * @brief Prints syntax error message and list of recent tokens with highlighted error.
     * @param message A message to be printed.
     */
    void PrintSyntaxError(const char* message);

    /**
     * @brief Prints lex error message and list of recent tokens with highlighted error.
     * @param message A message to be printed.
     */
    void PrintLexicalError(const char* message);

    /**
     * @brief Prints usage error message and help for the user.
     */
    void PrintUsageError();

    /**
     * @brief Enables development debug print if -d program option is used.
     */
    void EnableDebugPrint();

    /**
     * @brief Turns off logger.
     */
    void TurnOff();

    /**
     * @brief Prints help with options for the user.
     */
    void PrintHelp();
};