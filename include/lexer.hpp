/**
 * @file lexer.hpp
 * @brief Lexer for the S-Lang Compiler
 *
 * This file contains the definition of the Lexer class, used in the S-Lang compiler.
 * The Lexer is responsible for converting a string of source code into a stream of tokens.
 * These tokens are then used by the parser to create an abstract syntax tree.
 *
 * Usage involves creating an instance of Lexer with source code and calling the
 * `get_token` method to retrieve tokens one at a time.
 *
 * @author Sagar Patel
 * @date 12-3-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef LEXER_HPP
#define LEXER_HPP
#pragma once

#include "debug_stream.hpp"
#include "exceptions.hpp"
#include <string>
#include <unordered_map>

/**
 * @brief Enum class representing different types of tokens in S-Lang.
 */
enum class TokenType {
    // Types
    INT,
    FLOAT,
    BOOL,
    CHAR,
    STRING,

    // Keywords
    ARRAY,
    DEF,
    EXTERN,
    OPERATOR,
    PROGRAM,
    LET,
    IDENTIFIER,

    // Control Flow
    IF,
    ELSEIF,
    ELSE,
    WHILE,
	FOR,
    BREAK,
    CONTINUE,
    RETURN,
    END_OF_FILE,
	EXITCODE,

    // Complex
    COMPLEX,
};

// A Token alias, consisting of a TokenType and its associated string value.
using Token = std::pair<TokenType, std::string>;

/**
 * @brief The Lexer class for the S-Lang compiler.
 *
 * Lexer is responsible for breaking down the source code into tokens.
 * It reads the input source code string and outputs tokens that represent
 * the smallest individual units of the code, identifiable by the TokenType enum.
 */
class Lexer {
  private:
    std::string code;         // Source code as a string
    int current_char;         // Current character being processed
    std::string::iterator it; // Iterator for stepping through code string
    const std::unordered_map<std::string, TokenType> table =
        { // Keyword table (kept Tokens more readable for easier debugging)
            {"pluh", TokenType::DEF},
            {"plug", TokenType::EXTERN},
            {"cookUp", TokenType::LET},
            {"fr?", TokenType::IF},
            {"ong?", TokenType::ELSEIF},
            {"justLikeThat?", TokenType::ELSE},
            {"holdUp", TokenType::WHILE},
            {"ratioed", TokenType::FOR},
            {"ghost", TokenType::BREAK},
            {"rizz", TokenType::CONTINUE},
            {"periodt", TokenType::RETURN},
            {"facts", TokenType::BOOL},
            {"cap", TokenType::BOOL},
            {"spillingTheTeaAbout", TokenType::PROGRAM},
            {"gang", TokenType::ARRAY},
            {"yeet", TokenType::EXITCODE}};
  public:
    /**
     * @brief Constructs a new Lexer object with given source code.
     *
     * @param code The source code as a string.
     */
    Lexer(const std::string& code);

    /**
     * @brief Default move constructor.
     */
    Lexer(Lexer&&) = default;

    /**
     * @brief Checks if a given string is a keyword.
     *
     * This function is used to determine whether a given string is a keyword in the
     * context of the Lexer. Keywords are reserved words in the Slanguage so this
     * function allows you to identify them!
     *
     * @param keyword The string to check for being a keyword.
     * @return true if the provided string is a keyword, false otherwise.
     */
    bool is_keyword(const std::string& keyword);

    /**
     * @brief Retrieves the next token from the source code.
     *
     * Processes the source code and returns the next token. Continues from
     * the current position in the code string.
     *
     * @return The upcoming Token in the source code.
     */
    Token get_token();

    /**
     * @brief Default destructor.
     */
    ~Lexer() = default;
};

#endif