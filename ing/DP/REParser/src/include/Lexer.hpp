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
    bool m_inCharClass = false;

    uint32_t getNameLength(uint32_t groupNameStartPos) const;
    TokenType parseFourthCharInCaptureGroup(uint32_t& tokenLength) const;
    TokenType parseThirdCharInCaptureGroup(uint32_t& tokenLength) const;
    TokenType getCaptureGroupToken(uint32_t& tokenLength) const;
    TokenType getEscapeSequenceToken(uint32_t& tokenLength) const;
    TokenType getStandardTokenType(uint32_t& tokenLength, char currentChar);

public:
    void Initialize(const std::string& inputRegex);
    Token GetNextToken();
};