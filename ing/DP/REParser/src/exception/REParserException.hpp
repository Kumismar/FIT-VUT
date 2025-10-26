#pragma once
#include <string>

class REParserException
{
public:
    virtual ~REParserException();
    std::string what() const noexcept
    {
        return m_message;
    }

protected:
    std::string m_message;
};