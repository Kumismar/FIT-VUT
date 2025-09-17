/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-02 12:02
 * @ Modified time: 2024-05-02 10:43
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when there is an error in command line arguments.
 */
class CLArgumentsError : public ExceptionBase
{
public:
    /**
     * @brief Constructor for CLArgumentsError.
     * 
     * @param message Message that will be shown when the exception is thrown.
     */
    explicit CLArgumentsError(const char* message)
        : ExceptionBase(message)
    {
    }
};
