/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-23 19:32
 * @ Modified time: 2024-05-02 10:48
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when there are invalid command line arguments.
 */
class InvalidCmdArguments : public ExceptionBase
{
public:
    /**
     * @brief Constructor for InvalidCmdArguments.
     * 
     * @param message Message that will be shown when the exception is thrown.
     */
    InvalidCmdArguments(const std::string& message)
        : ExceptionBase(message)
    {
    }
};