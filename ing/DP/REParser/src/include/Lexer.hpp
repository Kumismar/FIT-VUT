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

public:
    void Initialize(std::string& inputRegex);
    Token GetNextToken();
};