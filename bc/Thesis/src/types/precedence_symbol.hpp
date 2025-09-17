/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 11:04
 */

#pragma once

#include "stack_item.hpp"
#include <string>


/**
 * @brief An enum representing all symbols from precedence table.
 */
typedef enum precedenceType
{
    Push, // <
    Reduce, // >
    Equal, // =
    Invalid // x
} PrecedenceType;


/**
 * @brief A class representing a symbol from precedence table.
 */
class PrecedenceSymbol : public Symbol
{
private:
    /**
     * @brief Type of the symbol.
     */
    PrecedenceType precType = PrecedenceType::Invalid;

public:
    /**
     * @brief PrecedenceSymbol constructor using type.
     */
    explicit PrecedenceSymbol(PrecedenceType type);

    /**
     * @brief PrecedenceSymbol copy constructor.
     */
    PrecedenceSymbol(const PrecedenceSymbol& old);

    /**
     * @brief PrecedenceSymbol destructor overriding Symbol destructor.
     */
    ~PrecedenceSymbol() override = default;

    /**
     * @brief Precedence symbol equality operator.
     * @details Compares two precedence symbols by their types.
     *
     * @param other The precedence symbol type to compare with.
     * @return True if the types are equal, false otherwise.
     */
    bool operator==(const PrecedenceType& type) const;

    /**
     * @brief Precedence symbol equality operator.
     * @details Compares two precedence symbols by their types.
     *
     * @param other The precedence symbol to compare with.
     * @return True if the types are equal, false otherwise.
     */
    bool operator==(const PrecedenceSymbol& other) const;

    /**
     * @brief Get the type of the symbol.
     * 
     * @return Type of the symbol.
     */
    PrecedenceType GetPrecedenceType() const;

    /**
     * @brief Get type of the symbol as string.
     * 
     * @return The type as string.
     */
    std::string GetTypeString() const override;

    /**
     * @brief Clone the symbol.
     * 
     * @return A new instance of the precedence symbol.
     */
    Symbol* Clone() const override;
};