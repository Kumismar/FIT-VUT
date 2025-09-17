/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-04 21:33
 * @ Modified time: 2024-05-02 10:47
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when there is an internal error of the program.
 * @details It's thrown when there is an error that is not caused by the user.
 * Never should be thrown.
 */
class InternalError : public ExceptionBase
{
public:
    explicit InternalError(const std::string& message)
        : ExceptionBase(message)
    {
    }
};