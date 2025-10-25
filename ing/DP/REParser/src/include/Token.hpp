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
    DOT,
    GROUP_START,
    GROUP_END,
    GROUP_NONCAPTURE_START,
    LOOKAHEAD_POSITIVE_START,
    LOOKAHEAD_NEGATIVE_START,
    LOOKBEHIND_POSITIVE_START,
    LOOKBEHIND_NEGATIVE_START,
    GROUP_NAMED_START,
    END_OF_INPUT
} TokenType;

struct Token
{
    TokenType type;
    std::string_view text;
};