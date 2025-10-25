/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "Lexer.hpp"

#include "Token.hpp"

#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

const uint32_t CLOSING_ANGLE_BRACKET_OFFSET = 1;

uint32_t Lexer::getNameLength(uint32_t groupNameStartPos)
{
    uint32_t nameLength = 0;
    uint32_t currentPos = groupNameStartPos;
    bool foundClosingBracket = false;

    while (currentPos < m_regex.length()) {
        char currentChar = m_regex[currentPos];

        if (currentChar == '>') {
            foundClosingBracket = true;
            break;
        }

        // Any alphanumeric symbol along with _ and $ are allowed
        if (!std::isalnum(currentChar) && currentChar != '_' && currentChar != '$') {
            // TODO: Create solid exception system
            throw std::runtime_error("Invalid character in capture group name");
        }
        nameLength++;
        currentPos++;
    }

    if (!foundClosingBracket) {
        throw std::runtime_error("Unclosed group capture name");
    }

    return nameLength;
}

void Lexer::Initialize(std::string& inputRegex)
{
    m_regex = inputRegex;
    m_position = 0;
}

TokenType Lexer::parseFourthCharInCaptureGroup(uint32_t& tokenLength)
{
    TokenType tokenType;
    const uint32_t fourthCharPos = m_position + 3;
    char fourthChar = m_regex[fourthCharPos];

    switch (fourthChar) {
        case '=': {
            tokenType = TokenType::LOOKBEHIND_POSITIVE_START;
            tokenLength = 4;
            break;
        }
        case '!': {
            tokenType = TokenType::LOOKBEHIND_NEGATIVE_START;
            tokenLength = 4;
            break;
        }
        default: {
            tokenType = TokenType::GROUP_NAMED_START;
            tokenLength = 3 + getNameLength(fourthCharPos) + CLOSING_ANGLE_BRACKET_OFFSET;
            break;
        }
    }
    return tokenType;
}

TokenType Lexer::parseThirdCharInCaptureGroup(uint32_t& tokenLength)
{
    TokenType tokenType;
    const uint32_t thirdCharPos = m_position + 2;
    char thirdChar = m_regex[thirdCharPos];

    switch (thirdChar) {
        case ':': {
            tokenType = TokenType::GROUP_NONCAPTURE_START;
            tokenLength = 3;
            break;
        }
        case '=': {
            tokenType = TokenType::LOOKAHEAD_POSITIVE_START;
            tokenLength = 3;
            break;
        }
        case '!': {
            tokenType = TokenType::LOOKAHEAD_NEGATIVE_START;
            tokenLength = 3;
            break;
        }
        case '<': {
            if (m_position + 3 >= m_regex.length()) {
                throw std::runtime_error("Unfinished sequence '(?<' at the end of regex");
            }

            tokenType = parseFourthCharInCaptureGroup(tokenLength);
            break;
        }
        default: {
            throw std::runtime_error("Invalid group indentifier");
        }
    }
    return tokenType;
}

Token Lexer::GetNextToken()
{
    if (m_position >= m_regex.length()) {
        return { TokenType::END_OF_INPUT, "" };
    }

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
        case '.': {
            tokenType = TokenType::DOT;
            break;
        }
        case '(': {
            if (m_position + 1 >= m_regex.length()) {
                // If '(' is present in the end of regex, we pass it to parser as valid token and
                // let the parser throw an error
                tokenType = TokenType::GROUP_START;
                break;
            }

            const uint32_t secondCharPos = m_position + 1;

            char secondChar = m_regex[secondCharPos];
            if (secondChar != '?') {
                tokenType = TokenType::GROUP_START;
                break;
            }

            if (m_position + 2 >= m_regex.length()) {
                throw std::runtime_error("Unfinished sequence '(?' at the end of regex");
            }

            tokenType = parseThirdCharInCaptureGroup(tokenLength);
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