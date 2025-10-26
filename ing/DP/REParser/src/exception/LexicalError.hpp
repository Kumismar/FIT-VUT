#pragma once
#include "REParserException.hpp"

class LexicalError final: public REParserException
{
public:
    explicit LexicalError(const std::string& message)
    {
        m_message = "Lexical Error: " + message;
    }
};