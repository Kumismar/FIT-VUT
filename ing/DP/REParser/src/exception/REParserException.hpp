#pragma once
#include <string>

class REParserException
{
public:
    virtual ~REParserException() = default;

    std::string what() const noexcept
    {
        return m_message;
    }

protected:
    std::string m_message;
};