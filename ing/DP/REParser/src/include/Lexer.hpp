/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#pragma once

#include "Token.hpp"

#include <cstdint>
#include <memory>
#include <string_view>

class Lexer;

using LexerPtr = std::shared_ptr<Lexer>;

class Lexer
{
    std::string_view m_regex;
    uint32_t m_position;

    uint32_t getNameLength(uint32_t groupNameStartPos);
    TokenType parseFourthCharInCaptureGroup(uint32_t& tokenLength, uint32_t& tmpPosition);
    TokenType parseThirdCharInCaptureGroup(uint32_t& tokenLength, uint32_t& tmpPosition);

public:
    void Initialize(std::string& inputRegex);
    Token GetNextToken();
};