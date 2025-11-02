/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-11-02
 * @ Description:
 */

#pragma once
#include <cstdint>
#include <string_view>

class SequenceValidator
{
    inline bool isOctal(const char digit) const;
    std::string_view m_regex;
    uint32_t m_position;

public:
    SequenceValidator(const std::string_view regex, const uint32_t position);

    void ValidateHexEscapeSequence(uint32_t& tokenLength) const;
    void ValidateUnicodeEscapeSequence(uint32_t& tokenLength) const;
    void ValidateControlEscapeSequence(uint32_t& tokenLength) const;
    void ValidateNamedBackReference(uint32_t& tokenLength) const;
    void ValidateBackReference(uint32_t& tokenLength) const;
    void ValidateOctalEscapeSequence(uint32_t& tokenLength) const;
};