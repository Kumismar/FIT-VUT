/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-05-02 14:50
 */

#include "analysis_success.hpp"
#include "ast.hpp"
#include "cl_arguments_error.hpp"
#include "exception_base.hpp"
#include "grammar_1.hpp"
#include "grammar_2.hpp"
#include "grammar_3.hpp"
#include "grammar_4.hpp"
#include "grammar_5.hpp"
#include "grammar_6.hpp"
#include "help.hpp"
#include "internal_error.hpp"
#include "lex.yy.h"
#include "lexical_error.hpp"
#include "logger.hpp"
#include "predictive_parser.hpp"
#include "syntax_error.hpp"
#include "token.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <unistd.h>

/**
 * @brief Performs memory cleanup after program run.
 */
void Cleanup()
{
    Grammar1::Cleanup();
    Grammar2::Cleanup();
    Grammar3::Cleanup();
    Grammar4::Cleanup();
    Grammar5::Cleanup();
    Grammar6::Cleanup();
    Logger::Cleanup();
    AST::Cleanup();

    if (!inputTape.empty()) {
        for (const auto& token: inputTape) {
            delete token;
        }
        inputTape.clear();
    }
}

/**
 * @brief Calls generated lexer to tokenize the input file.
 * 
 * @param filename Name of the input file. 
 */
void Lex(std::string& filename)
{
    namespace fs = std::filesystem;

    fs::path toOpen(filename);
    if (!fs::exists(toOpen)) {
        throw CLArgumentsError("Input file not found.\n");
    }

    if ((yyin = fopen(toOpen.c_str(), "r")) == nullptr) {
        throw InternalError("Failed to open input file.\n");
    }
    yylex();
    yylex_destroy();
    inputTape.push_back(new Token(t_End));
    Logger::GetInstance()->PrintInputTape();
}

/**
 * @brief Processes command line arguments.
 * 
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Name of the input file.
 */
std::string ProcessArguments(int argc, char** argv)
{
    std::string filename;
    int opt;
    bool fileOption = false;

    while ((opt = getopt(argc, argv, "df:th")) != -1) {
        switch (opt) {
            case 'd': {
                Logger::GetInstance()->EnableDebugPrint();
                break;
            }
            case 'f': {
                filename = optarg;
                fileOption = true;
                break;
            }
            case 't': {
                AST::GetInstance()->SetTreeFlag();
                break;
            }
            case 'h': {
                throw Help();
            }
            default: {
                throw CLArgumentsError("Invalid argument.\n");
            }
        }
    }

    if (!fileOption) {
        throw CLArgumentsError("No input file specified.\n");
    }

    return filename;
}

int main(int argc, char** argv)
{
    AnalysisStack stackos;
    int retCode = 0;
    PredictiveParser predParser(stackos);

    try {
        std::string filename = ProcessArguments(argc, argv);
        Lex(filename);
        predParser.InitSyntaxAnalysis();
        predParser.Parse(false);
    }
    catch (SyntaxAnalysisSuccess const& e) {
        AST::GetInstance()->PrintTree();
        std::cout << "Parsing successful." << std::endl;
        retCode = 0;
    }
    catch (Help const& e) {
        Logger::GetInstance()->PrintHelp();
        retCode = 0;
    }
    catch (SyntaxError const& e) {
        Logger::GetInstance()->PrintSyntaxError(e.what());
        retCode = 1;
    }
    catch (LexicalError const& e) {
        Logger::GetInstance()->PrintLexicalError(e.what());
        retCode = 2;
    }
    catch (CLArgumentsError const& e) {
        Logger::GetInstance()->PrintUsageError();
        retCode = 3;
    }
    catch (InternalError const& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        retCode = 99;
    }
    catch (ExceptionBase const& e) {
        std::cerr << "Uncatched exception: " << e.what() << std::endl;
        retCode = -1;
    }
    catch (std::exception const& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        retCode = -1;
    }
    catch (...) {
        std::cerr << "Unknown error." << std::endl;
        retCode = -1;
    }

    predParser.ClearStack();
    Cleanup();
    return retCode;
}