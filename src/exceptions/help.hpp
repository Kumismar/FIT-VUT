/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-30 00:54
 * @ Modified time: 2024-05-02 10:46
 */

#pragma once

#include "exception_base.hpp"

/**
 * @brief Exception thrown when -h option is present.
 */
class Help : public ExceptionBase
{
};