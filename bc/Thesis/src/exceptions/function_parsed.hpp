/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-23 12:54
 * @ Modified time: 2024-05-02 10:46
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when function is parsed.
 * @details Is used for communication between predictive and precedence parser.
 * It's thrown by predictive parser when t_FuncEnd token is found.
 */
class FunctionParsed : public ExceptionBase
{
};