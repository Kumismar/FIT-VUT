/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "Lexer.hpp"

#include "LexicalError.hpp"
#include "Token.hpp"

#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

constexpr uint32_t CLOSING_ANGLE_BRACKET_OFFSET = 1;

uint32_t Lexer::getNameLength(uint32_t groupNameStartPos) const
{
    uint32_t nameLength = 0;
    uint32_t currentPos = groupNameStartPos;
    bool foundClosingBracket = false;

    while (currentPos < m_regex.length()) {
        const char currentChar = m_regex[currentPos];

        if (currentChar == '>') {
            foundClosingBracket = true;
            break;
        }

        // Any alphanumeric symbol along with _ and $ are allowed
        if (!std::isalnum(currentChar) && currentChar != '_' && currentChar != '$') {
            throw LexicalError("Invalid character in capture group name at position " +
                               std::to_string(currentPos) + " in regex");
        }
        nameLength++;
        currentPos++;
    }

    if (!foundClosingBracket) {
        throw LexicalError("Unclosed group capture name at position " + std::to_string(currentPos) +
                           " in regex");
    }

    return nameLength;
}

TokenType Lexer::parseFourthCharInCaptureGroup(uint32_t& tokenLength) const
{
    TokenType tokenType;
    const uint32_t fourthCharPos = m_position + 3;
    const char fourthChar = m_regex[fourthCharPos];

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

TokenType Lexer::parseThirdCharInCaptureGroup(uint32_t& tokenLength) const
{
    TokenType tokenType;
    const uint32_t thirdCharPos = m_position + 2;
    const char thirdChar = m_regex[thirdCharPos];

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
                throw LexicalError("Unfinished sequence '(?<' at position" +
                                   std::to_string(m_position + 2) + " in regex");
            }

            tokenType = parseFourthCharInCaptureGroup(tokenLength);
            break;
        }
        default: {
            throw LexicalError("Invalid group indentifier" + std::to_string(m_position + 1) +
                               " in regex");
        }
    }
    return tokenType;
}

TokenType Lexer::getCaptureGroupToken(uint32_t& tokenLength) const
{
    if (m_position + 1 >= m_regex.length()) {
        // If '(' is present in the end of regex, we pass it to parser as valid token and
        // let the parser throw an error
        return TokenType::GROUP_START;
    }

    const uint32_t secondCharPos = m_position + 1;

    const char secondChar = m_regex[secondCharPos];
    if (secondChar != '?') {
        // When '?' is not the second character, then we treat '(' as start of group and next
        // character is inside the group, thus the token has length 1
        return TokenType::GROUP_START;
    }

    if (m_position + 2 >= m_regex.length()) {
        throw LexicalError("Unfinished sequence '(?' at the end of regex");
    }

    return parseThirdCharInCaptureGroup(tokenLength);
}

TokenType Lexer::getEscapeSequenceToken(uint32_t& tokenLength)
{
    if (m_position + 1 >= m_regex.length()) {
        throw LexicalError("Unfinished escape sequence at the end of regex");
    }

    const char secondChar = m_regex[m_position + 1];
    tokenLength = 2;

    switch (secondChar) {
        // Character classes
        case 'd': {
            return TokenType::DIGIT_CLASS;
        }
        case 'D': {
            return TokenType::NON_DIGIT_CLASS;
        }
        case 'w': {
            return TokenType::WORD_CHAR_CLASS;
        }
        case 'W': {
            return TokenType::NON_WORD_CHAR_CLASS;
        }
        case 's': {
            return TokenType::WHITESPACE_CLASS;
        }
        case 'S': {
            return TokenType::NON_WHITESPACE_CLASS;
        }

        // Anchors
        case 'b': {
            return TokenType::ANCHOR_WORD_BOUNDARY;
        }
        case 'B': {
            return TokenType::ANCHOR_NONWORD_BOUNDARY;
        }

        // Hexadecimal and unicode escape sequence
        case 'x': {
            // 'xHH'
            // Bude potřeba přečíst další 2 znaky a upravit tokenLength na 4.
            // Prozatím můžeme vrátit LITERAL.
            tokenLength = 4;  // Musíš zkontrolovat, jestli máš 2 další znaky!
            return TokenType::LITERAL;
        }
        case 'u': {
            // 'xHHHH' or 'x{...}'
            // Extrémně složitá logika parsování.
            // Prozatím můžeme vrátit LITERAL.
            return TokenType::LITERAL;
        }

        // Control + [A-Z] characters
        case 'c': {
            // Bude potřeba přečíst další 1 znak a upravit tokenLength na 3.
            tokenLength = 3;  // Musíš zkontrolovat, jestli máš další znak!
            return TokenType::LITERAL;
        }

        // Backreferences
        case 'k': {
            // TODO: Zpracování pojmenované zpětné reference '\k<name>'
            // Bude potřeba parsovat jméno v závorkách.
            // tokenLength bude 3 + délka jména + 1.
            return TokenType::NAMED_BACKREFERENCE;  // Nebo podobný token
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            // TODO: Zpracování zpětné reference (může být víceciferná)
            // Tohle NENÍ literál, je to speciální token.
            return TokenType::BACKREFERENCE;
        }

        default: {
            // This branch includes:
            // - Escaped meta characters: '\*', '\+', '\?', '\(', '\[', '\{', '\.', '\\', etc.
            // - Special characters: '\f', '\n', '\r', '\t', '\v', '\0'
            // - Unknown escape sequences, e.g. '\a', '\z', that mean 'a', 'z', etc.
            // All of them are literals of length 2
            return TokenType::LITERAL;
        }
    }
}

TokenType Lexer::getStandardTokenType(uint32_t& tokenLength, const char currentChar)
{
    switch (currentChar) {
        case '*': {
            return TokenType::QUANT_STAR;
        }
        case '+': {
            return TokenType::QUANT_PLUS;
        }
        case '?': {
            return TokenType::QUANT_QUESTION_MARK;
        }
        case '{': {
            return TokenType::QUANT_BRACE_START;
        }
        case '}': {
            return TokenType::QUANT_BRACE_END;
        }
        case '.': {
            return TokenType::DOT;
        }
        case '|': {
            return TokenType::ALTERNATION;
        }
        case '^': {
            return TokenType::ANCHOR_START;
        }
        case '$': {
            return TokenType::ANCHOR_END;
        }
        case '(': {
            return getCaptureGroupToken(tokenLength);
        }
        case '\\': {
            return getEscapeSequenceToken(tokenLength);
        }
        case '[': {
            m_inCharClass = true;
            return TokenType::CHAR_CLASS_START;
        }
        case ']': {
            m_inCharClass = false;
            return TokenType::CHAR_CLASS_END;
        }
        default: {
            return TokenType::LITERAL;
        }
    }
}

void Lexer::Initialize(const std::string& inputRegex)
{
    m_regex = inputRegex;
    m_position = 0;
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

    if (m_inCharClass) {
    }
    else {
        tokenType = getStandardTokenType(tokenLength, currentChar);
    }

    m_position += tokenLength;
    return { tokenType, std::string_view(tokenStart, tokenLength) };
}