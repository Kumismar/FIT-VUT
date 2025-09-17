/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-02-05 17:44
 * @ Modified time: 2024-05-02 10:41
 */

#include "exception_base.hpp"

ExceptionBase::ExceptionBase(const std::string& m)
    : message(m)
{
}

const char* ExceptionBase::what() const noexcept
{
    return this->message.c_str();
}