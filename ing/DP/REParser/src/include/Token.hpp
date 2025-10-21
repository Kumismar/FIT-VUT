/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#pragma once
#include <string_view>

typedef enum TokenType
{
    LITERAL,
    QUANT_STAR,
    QUANT_PLUS,
    QUANT_QUESTION_MARK,
    QUANT_BRACE_START,
    QUANT_BRACE_END,
} TokenType;

struct Token
{
    TokenType type;
    std::string_view text;
};