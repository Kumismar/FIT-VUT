/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-22 22:14
 * @ Modified time: 2024-05-02 11:13
 */

#pragma once

#include <string>

/**
 * @brief An enum representing symbol types.
 */
typedef enum symbolType
{
    symb_Token,
    symb_Nonterminal,
    symb_PrecSymbol
} SymbolType;


/**
 * @brief A class representing a symbol in the stack.
 */
class Symbol
{
protected:
    /**
     * @brief Type of the symbol.
     */
    SymbolType symbType;

public:
    /**
     * @brief Virtual symbol destructor.
     */
    virtual ~Symbol() = default;

    /**
     * @brief Symbol equality operator.
     * @details First checks class types. 
     * If class types are equal, derived class equality operator on types is called.
     *
     * @param other The symbol to compare with.
     * @return True if the symbols are equal, false otherwise.
     */
    bool operator==(const Symbol& other) const;

    /**
     * @brief Symbol inequality operator.
     * @details Calls equality operator and negates the result.
     *
     * @param other The symbol to compare with.
     * @return True if the symbols are not equal, false otherwise.
     */
    bool operator!=(const Symbol& other) const;

    /**
     * @brief Get the type of the symbol.
     *
     * @return The type of symbol.
     */
    SymbolType GetSymbolType() const;

    /**
     * @brief Get the type of the symbol as a string.
     * Derived classes use this method to return their type as a string.
     *
     * @return The type of symbol as a string.
     */
    virtual std::string GetTypeString() const = 0;

    /**
     * @brief Clone the symbol.
     * Derived classes use this method to clone themselves.
     *
     * @return A pointer to the cloned symbol.
     */
    virtual Symbol* Clone() const = 0;
};