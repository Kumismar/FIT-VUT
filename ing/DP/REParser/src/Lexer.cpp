/**
 * @Author: Ondřej Koumar
 * @Project: REParser
 * @Create Date: 2025-10-21
 * @Description: Implemenation of the Lexer class methods.
 */

#include "Lexer.hpp"

#include "LexicalError.hpp"
#include "Token.hpp"

#include <string>
#include <string_view>

constexpr uint32_t CLOSING_ANGLE_BRACKET_OFFSET = 1;
constexpr uint32_t BACKSLASH_OFFSET = 1;
constexpr uint32_t NAMED_BACKREF_START_OFFSET = 3;
constexpr uint32_t HEX_SEQUENCE_LENGTH = 3;
constexpr uint32_t CONTROL_SEQUENCE_LENGTH = 1;
constexpr uint32_t NAMED_BACKREF_MINIMAL_LENGTH = 4;

bool isOctal(const char digit)
{
    return digit >= '0' && digit <= '7';
}

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
            throw LexicalError("Invalid group indentifier at position" +
                               std::to_string(m_position + 1) + " in regex");
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

void Lexer::validateHexEscapeSequence(uint32_t& tokenLength) const
{
    if (m_position + HEX_SEQUENCE_LENGTH >= m_regex.length()) {
        throw LexicalError("Unfinished hexadecimal escape sequence at the end of regex");
    }

    const char firstHexDigit = m_regex[m_position + 2];
    const char secondHexDigit = m_regex[m_position + 3];

    if (!std::isxdigit(static_cast<unsigned char>(firstHexDigit)) ||
        !std::isxdigit(static_cast<unsigned char>(secondHexDigit))) {
        throw LexicalError("Invalid hexadecimal escape sequence");
    }

    tokenLength = 4;
}

void Lexer::validateUnicodeEscapeSequence(uint32_t& tokenLength) const
{
    // 'xHHHH' or 'x{...}'
    // TODO: parsing unicode escape sekvencí
}

void Lexer::validateControlEscapeSequence(uint32_t& tokenLength) const
{
    if (m_position + CONTROL_SEQUENCE_LENGTH >= m_regex.length()) {
        throw LexicalError("Unfinished control escape sequence at the end of regex");
    }

    const char controlChar = m_regex[m_position + 1];

    // Only [A-Z] characters allowed
    if (!std::isupper(static_cast<unsigned char>(controlChar))) {
        throw LexicalError("Invalid control escape sequence");
    }

    tokenLength = 3;
}

void Lexer::validateNamedBackReference(uint32_t& tokenLength) const
{
    // '\k<name>' ---> tokenLength = 3 + len(name) + 1, where name is nonempty string
    // ---> at least 5 chars in total, further checks done when resolving name
    // currently at '\' ---> 4 more at least to go
    if (m_position + NAMED_BACKREF_MINIMAL_LENGTH >= m_regex.length()) {
        throw LexicalError("Invalid named backreference at the end of regex");
    }

    const char openBracketChar = m_regex[m_position + 2];
    if (openBracketChar != '<') {
        throw LexicalError("Missing open angle bracket in named back reference");
    }

    const uint32_t nameStartPos = m_position + 3;
    uint32_t nameLength = 0;
    bool foundClosingBracket = false;

    // Go through the back reference name char by char and count the length of it.
    for (uint32_t currentPos = nameStartPos; currentPos < m_regex.length(); currentPos++) {
        const char currentNameChar = m_regex[currentPos];

        if (currentNameChar == '>') {
            foundClosingBracket = true;
            break;
        }

        if (!std::isalnum(static_cast<unsigned char>(currentNameChar)) && currentNameChar != '_' &&
            currentNameChar != '$') {
            throw LexicalError("Invalid character in back reference name");
        }

        nameLength++;
    }

    if (!foundClosingBracket) {
        throw LexicalError("Unclosed back reference name");
    }

    if (nameLength == 0) {
        throw LexicalError("Empty back reference name");
    }

    tokenLength = NAMED_BACKREF_START_OFFSET + nameLength + CLOSING_ANGLE_BRACKET_OFFSET;
}

void Lexer::validateBackReference(uint32_t& tokenLength) const
{
    uint32_t numOfDigits = 1;
    const uint32_t startPos = m_position + 2;
    uint32_t currentPos = startPos;

    while (currentPos < m_regex.length() &&
           std::isdigit(static_cast<unsigned char>(m_regex[currentPos]))) {
        numOfDigits++;
        currentPos++;
    }

    tokenLength = numOfDigits + BACKSLASH_OFFSET;
}

void Lexer::validateOctalEscapeSequence(uint32_t& tokenLength) const
{
    // An octal number can be one, two or three digits long
    if (m_position + 2 >= m_regex.length()) {
        return;
    }

    const char secondDigit = m_regex[m_position + 2];
    if (!isOctal(secondDigit)) {
        return;
    }
    tokenLength = 3;

    // Octal sequences in ECMAScript regexes are valid up to '\377'.
    // A regex '\402' matches literal '\40' in octal and then '2' in decimal.
    const char firstDigit = m_regex[m_position + 1];
    if (firstDigit > '3') {
        return;
    }

    if (m_position + 3 >= m_regex.length()) {
        return;
    }

    const char thirdDigit = m_regex[m_position + 3];
    if (!isOctal(thirdDigit)) {
        return;
    }

    tokenLength = 4;
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
            validateHexEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'u': {
            validateUnicodeEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }

        // Control + [A-Z] characters
        case 'c': {
            validateControlEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }

        // Backreferences
        case 'k': {
            validateNamedBackReference(tokenLength);
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
            validateBackReference(tokenLength);
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
            validateHexEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'u': {
            validateUnicodeEscapeSequence(tokenLength);
            return TokenType::LITERAL;
        }
        case 'c': {
            validateControlEscapeSequence(tokenLength);
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
            validateOctalEscapeSequence(tokenLength);
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
            return TokenType::CHAR_CLASS_HYPHEN;
        }
        case '^': {
            return TokenType::CHAR_CLASS_CARET;
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

    TokenType tokenType = TokenType::END_OF_INPUT;
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