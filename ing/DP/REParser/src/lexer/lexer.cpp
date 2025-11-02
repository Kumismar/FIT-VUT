/**
 * @Author: Ondřej Koumar
 * @Project: REParser
 * @Create Date: 2025-10-21
 * @Description: Implemenation of the Lexer class methods.
 */

#include "lexer.hpp"

#include "LexicalError.hpp"
#include "sequence_validator.hpp"
#include "token.hpp"

#include <string>
#include <string_view>

constexpr uint32_t CLOSING_ANGLE_BRACKET_OFFSET = 1;

uint32_t Lexer::getNameLength(const uint32_t groupNameStartPos) const
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
        if (!std::isalnum(static_cast<unsigned char>(currentChar)) && currentChar != '_' &&
            currentChar != '$') {
            throw LexicalError("Invalid character in capture group name at position " +
                               std::to_string(currentPos + 1) + " in regex");
        }
        nameLength++;
        currentPos++;
    }

    if (!foundClosingBracket) {
        throw LexicalError("Unclosed group capture name at position " +
                           std::to_string(currentPos + 1) + " in regex");
    }

    return nameLength;
}

inline TokenType Lexer::parseFourthCharInCaptureGroup(uint32_t& tokenLength) const
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

inline TokenType Lexer::parseThirdCharInCaptureGroup(uint32_t& tokenLength) const
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
                                   std::to_string(m_position + 3) + " in regex");
            }

            tokenType = parseFourthCharInCaptureGroup(tokenLength);
            break;
        }
        default: {
            throw LexicalError("Invalid group indentifier at position" +
                               std::to_string(m_position + 2) + " in regex");
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

TokenType Lexer::getEscapeSequenceToken(uint32_t& tokenLength) const
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
            SequenceValidator(m_regex, m_position).ValidateHexEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'u': {
            SequenceValidator(m_regex, m_position).ValidateUnicodeEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }

        // Control + [A-Z] characters
        case 'c': {
            SequenceValidator(m_regex, m_position).ValidateControlEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }

        // Backreferences
        case 'k': {
            SequenceValidator(m_regex, m_position).ValidateNamedBackReference(tokenLength);
            return TokenType::NAMED_BACKREFERENCE;
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
            SequenceValidator(m_regex, m_position).ValidateBackReference(tokenLength);
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
        case ',': {
            return TokenType::COMMA;
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
        case ')': {
            return TokenType::GROUP_END;
        }
        case '\\': {
            return getEscapeSequenceToken(tokenLength);
        }
        case '[': {
            m_inCharClass = true;
            return TokenType::CHAR_CLASS_START;
        }
        default: {
            return TokenType::LITERAL;
        }
    }
}

TokenType Lexer::getCharClassEscapeSequenceToken(uint32_t& tokenLength) const
{
    if (m_position + 1 >= m_regex.length()) {
        throw LexicalError("Unfinished escape sequence at the end of regex");
    }

    const char secondChar = m_regex[m_position + 1];
    tokenLength = 2;

    switch (secondChar) {
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
        case 'x': {
            SequenceValidator(m_regex, m_position).ValidateHexEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'u': {
            SequenceValidator(m_regex, m_position).ValidateUnicodeEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'c': {
            SequenceValidator(m_regex, m_position).ValidateControlEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7': {
            // IMPORTANT: This is only for non-unicode regexes only. For regexes with '/u' modifier,
            // an octal sequence in character class is completely invalid.
            SequenceValidator(m_regex, m_position).ValidateOctalEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        default: {
            return TokenType::LITERAL;
        }
    }
}

TokenType Lexer::getTokenTypeFromCharClass(uint32_t& tokenLength, const char currentChar)
{
    switch (currentChar) {
        case ']': {
            m_inCharClass = false;
            return TokenType::CHAR_CLASS_END;
        }
        case '-': {
            return TokenType::CHAR_CLASS_RANGE;
        }
        case '^': {
            return TokenType::CHAR_CLASS_NEGATION;
        }
        case '\\': {
            return getCharClassEscapeSequenceToken(tokenLength);
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
    const char currentChar = *tokenStart;

    TokenType tokenType;
    uint32_t tokenLength = 1;

    if (m_inCharClass) {
        // Processing characters between '[' and ']'
        tokenType = getTokenTypeFromCharClass(tokenLength, currentChar);
    }
    else {
        tokenType = getStandardTokenType(tokenLength, currentChar);
    }

    m_position += tokenLength;
    return { tokenType, std::string_view(tokenStart, tokenLength) };
}