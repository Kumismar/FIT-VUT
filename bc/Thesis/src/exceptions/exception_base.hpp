/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-02-05 17:38
 * @ Modified time: 2024-05-02 10:41
 */

#pragma once

#include <exception>
#include <string>

/**
 * @brief Base class for all custom exceptions that are thrown by the program.
 */
class ExceptionBase : public std::exception
{
private:
    /**
     * @brief Message that will be shown when the exception is thrown.
     */
    std::string message;

public:
    /**
     * @brief A constructor for ExceptionBase.
     */
    ExceptionBase() = default;

    /**
     * @brief A constructor for ExceptionBase.
     * 
     * @param m Message that will be shown when the exception is thrown.
     */
    explicit ExceptionBase(const std::string& m);

    /**
     * @brief Returns the message that will be shown when the exception is thrown.
     * 
     * @return const char* The message as c-style string.
     */
    const char* what() const noexcept override;
};