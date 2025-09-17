/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-11 12:42
 * @ Modified time: 2024-05-02 10:48
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when lex detects unknown lexeme.
 */
class LexicalError : public ExceptionBase
{
public:
    /**
     * @brief Constructor for LexicalError.
     * 
     * @param message Message that will be shown when the exception is thrown.
     */
    explicit LexicalError(const std::string& message)
        : ExceptionBase(message)
    {
    }
};