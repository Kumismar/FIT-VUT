/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "Lexer.hpp"

#include "Token.hpp"

#include <cctype>
#include <string>
#include <string_view>

void Lexer::Initialize(std::string& inputRegex)
{
    m_regex = inputRegex;
    m_position = 0;
}

Token Lexer::GetNextToken()
{
    const char* tokenStart = &m_regex[m_position];
    char currentChar = *tokenStart;

    TokenType tokenType;
    uint32_t tokenLength = 1;

    switch (currentChar) {
        case '*': {
            tokenType = TokenType::QUANT_STAR;
            break;
        }
        case '+': {
            tokenType = TokenType::QUANT_PLUS;
            break;
        }
        case '?': {
            tokenType = TokenType::QUANT_QUESTION_MARK;
            break;
        }
        case '{': {
            tokenType = TokenType::QUANT_BRACE_START;
            break;
        }
        case '}': {
            tokenType = TokenType::QUANT_BRACE_END;
            break;
        }
        default: {
            tokenType = TokenType::LITERAL;
            break;
        }
    }

    m_position += tokenLength;
    return { tokenType, std::string_view(tokenStart, tokenLength) };
}