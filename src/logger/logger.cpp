/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:05
 */

#include "logger.hpp"
#include "internal_error.hpp"
#include "stack_item.hpp"
#include "token.hpp"
#include <filesystem>
#include <iostream>
#include <ranges>

Logger* Logger::instance = nullptr;

Logger::Logger()
{
    namespace fs = std::filesystem;
    fs::path filePath = fs::path(__FILE__).parent_path().parent_path().parent_path() / "output.log";

    this->file.open(filePath, std::ios::out | std::ios::trunc);
    if (!this->file.is_open()) {
        throw InternalError("Couldn't open file for logging\n");
    }
}

Logger::~Logger()
{
    this->file.close();
}

void Logger::Cleanup()
{
    if (Logger::instance == nullptr) {
        return;
    }

    if (!Logger::instance->recentTokens.empty()) {
        for (const Token* token: Logger::instance->recentTokens) {
            delete token;
        }
        Logger::instance->recentTokens.clear();
    }

    delete Logger::instance;
    Logger::instance = nullptr;
}

Logger* Logger::GetInstance()
{
    if (Logger::instance == nullptr) {
        Logger::instance = new Logger();
    }
    return Logger::instance;
}

void Logger::AddLeftSide(Symbol* leftSide)
{
    if (this->turnedOff) {
        return;
    }
    this->leftSideRule = leftSide->Clone();
}

void Logger::AddRightSide(Rule& rightSide)
{
    if (this->turnedOff) {
        return;
    }

    for (Symbol* symbol: rightSide) {
        this->rightSideRule.push_front(symbol->Clone());
    }
}

void Logger::PrintRule()
{
    if (this->turnedOff) {
        return;
    }

    this->file << this->leftSideRule->GetTypeString() << " -> ";
    for (const Symbol* item: this->rightSideRule) {
        this->file << item->GetTypeString() << " ";
    }
    this->file << std::endl;
    this->clearRule();
}

void Logger::PrintInputTape() const
{
    if (!this->enableDebugPrint || this->turnedOff) {
        return;
    }

    size_t maxTypeLength = 0;
    for (const auto token: inputTape) {
        maxTypeLength = std::max(maxTypeLength, token->GetTypeString().size());
    }

    // Print tokens with fixed-width columns
    for (const auto token: inputTape) {
        std::cout << "type: " << std::setw(static_cast<int>(maxTypeLength)) << token->GetTypeString()
                  << "\tdata: " << token->GetDataString().c_str() << "\n";
    }
    std::cout << std::endl;
}

void Logger::AddTokenToRecents(Token& token)
{
    if (this->turnedOff) {
        return;
    }

    if (token == TokenType::t_FuncConst) {
        return;
    }

    this->recentTokens.push_front(token.Clone());
    if (this->recentTokens.size() > 10) {
        delete this->recentTokens.back();
        this->recentTokens.pop_back();
    }
}

void Logger::PrintSyntaxError(const char* message)
{
    if (this->turnedOff) {
        return;
    }

    if (this->recentTokens.empty()) {
        std::cerr << this->red << "File is empty!" << this->reset << std::endl;
        return;
    }

    std::cerr << this->red << "Syntax error "
              << this->reset << "near line " << std::to_string(this->recentTokens.front()->GetLineNo()) << ": " << message;

    // Get position of the error
    size_t position = 0;
    for (const Token* token: this->recentTokens) {
        position += token->GetDataString().size() + 1; // for space between tokens
    }

    // Print recent tokens
    for (const Token* recentToken: std::ranges::reverse_view(this->recentTokens)) {
        std::cerr << recentToken->GetDataString() << " ";
    }

    // Print max three more tokens from input tape
    int i = 0;
    for (const Token* token: inputTape) {
        if (i >= 3) {
            break;
        }
        std::cerr << token->GetDataString() << " ";
        i++;
    }

    // Print error position
    std::cerr << "\n";
    for (size_t j = 0; j < position; j++) {
        std::cerr << " ";
    }
    std::cerr << this->red << "^" << this->reset << std::endl;
}

void Logger::PrintLexicalError(const char* message)
{
    if (this->turnedOff) {
        return;
    }

    std::cerr << this->red << "Lexical error " << this->reset << message;
}

void Logger::PrintUsageError()
{
    if (this->turnedOff) {
        return;
    }

    std::cerr << "Usage: build/src/Parser " << this->underlined << "[-d] [-t] [-h]" << this->reset << " "
              << this->underlined << "-f" << this->reset << " " << this->underlined << "<filename>\n"
              << this->reset;
    this->PrintHelp();
}

void Logger::clearRule()
{
    delete this->leftSideRule;
    this->leftSideRule = nullptr;
    for (const Symbol* symbol: this->rightSideRule) {
        delete symbol;
    }
    this->rightSideRule.clear();
}

void Logger::EnableDebugPrint()
{
    this->enableDebugPrint = true;
}

void Logger::TurnOff()
{
    this->turnedOff = true;
}

void Logger::PrintHelp()
{
    std::cout << "Options:\n"
              << "\t" << this->underlined << "-d" << this->reset << "  Enable debug print - print program state to stdout (used in development)\n"
              << "\t" << this->underlined << "-t" << this->reset << "  Print AST\n"
              << "\t" << this->underlined << "-h" << this->reset << "  Print this help message\n"
              << "\t" << this->underlined << "-f FILENAME" << this->reset << "  Specify input file\n";
}