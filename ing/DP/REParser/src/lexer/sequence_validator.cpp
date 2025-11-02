
//
// Created by koumy on 02/11/2025.
//
#include "include/sequence_validator.hpp"

#include "LexicalError.hpp"
#include "sequence_validator.hpp"

#include <cctype>
#include <string>

constexpr uint32_t HEX_SEQUENCE_LENGTH = 3;
constexpr uint32_t CONTROL_SEQUENCE_LENGTH = 1;
constexpr uint32_t BACKSLASH_OFFSET = 1;
constexpr uint32_t NAMED_BACKREF_START_OFFSET = 3;
constexpr uint32_t NAMED_BACKREF_MINIMAL_LENGTH = 4;
constexpr uint32_t CLOSING_ANGLE_BRACKET_OFFSET = 1;

inline bool SequenceValidator::isOctal(const char digit) const
{
    return digit >= '0' && digit <= '7';
}

SequenceValidator::SequenceValidator(const std::string_view regex, const uint32_t position)
    : m_regex(regex),
      m_position(position)
{
}

void SequenceValidator::ValidateHexEscapeSequence(uint32_t& tokenLength) const
{
    if (m_position + HEX_SEQUENCE_LENGTH >= m_regex.length()) {
        throw LexicalError("Unfinished hexadecimal escape sequence at the end of regex");
    }

    const char firstHexDigit = m_regex[m_position + 2];
    const char secondHexDigit = m_regex[m_position + 3];

    if (!std::isxdigit(static_cast<unsigned char>(firstHexDigit)) ||
        !std::isxdigit(static_cast<unsigned char>(secondHexDigit))) {
        throw LexicalError("Invalid hexadecimal escape sequence at position " +
                           std::to_string(m_position + 2) + " in regex");
    }

    tokenLength = 4;
}

void SequenceValidator::ValidateUnicodeEscapeSequence(uint32_t& tokenLength) const
{
    // 'xHHHH' or 'x{...}'
    // TODO: parsing unicode escape sekvencí
}

void SequenceValidator::ValidateControlEscapeSequence(uint32_t& tokenLength) const
{
    if (m_position + CONTROL_SEQUENCE_LENGTH >= m_regex.length()) {
        throw LexicalError("Unfinished control escape sequence at the end of regex");
    }

    const char controlChar = m_regex[m_position + 2];

    // Only [A-Z] characters allowed
    if (!std::isupper(static_cast<unsigned char>(controlChar))) {
        throw LexicalError("Invalid control escape sequence at position " +
                           std::to_string(m_position + 2));
    }

    tokenLength = 3;
}

void SequenceValidator::ValidateNamedBackReference(uint32_t& tokenLength) const
{
    // '\k<name>' ---> tokenLength = 3 + len(name) + 1, where name is nonempty string
    // ---> at least 5 chars in total, further checks done when resolving name
    // currently at '\' ---> 4 more at least to go
    if (m_position + NAMED_BACKREF_MINIMAL_LENGTH >= m_regex.length()) {
        throw LexicalError("Invalid named backreference at the end of regex");
    }

    const char openBracketChar = m_regex[m_position + 2];
    if (openBracketChar != '<') {
        throw LexicalError("Missing open angle bracket in named back reference at position " +
                           std::to_string(m_position + 3) + " in regex");
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
            throw LexicalError("Invalid character in back reference name at position " +
                               std::to_string(currentPos + 1) + " in regex");
        }

        nameLength++;
    }

    if (!foundClosingBracket) {
        throw LexicalError("Unclosed back reference name at the end of regex");
    }

    if (nameLength == 0) {
        throw LexicalError("Empty back reference name at position " +
                           std::to_string(nameStartPos + 1) + " in regex");
    }

    tokenLength = NAMED_BACKREF_START_OFFSET + nameLength + CLOSING_ANGLE_BRACKET_OFFSET;
}

void SequenceValidator::ValidateBackReference(uint32_t& tokenLength) const
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

void SequenceValidator::ValidateOctalEscapeSequence(uint32_t& tokenLength) const
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