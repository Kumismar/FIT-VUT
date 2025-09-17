/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-02-11 19:32
 * @ Modified time: 2024-05-02 10:52
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when parsers find a syntax error.
 */
class SyntaxError : public ExceptionBase
{
public:
    /**
     * @brief Constructor for SyntaxError.
     * 
     * @param message Message that will be shown when the exception is thrown.
     */
    explicit SyntaxError(const std::string& message)
        : ExceptionBase(message)
    {
    }
};