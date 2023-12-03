#ifndef LEXER_HPP
#define LEXER_HPP
#pragma once

#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include "exceptions.hpp"

enum class TokenType {
    // Types
    INT, FLOAT, BOOL, STRING, CHAR, POINTER,
    
    // Keywords
    DEF, EXTERN, OPERATOR, MODULE, LET, IDENTIFIER,

    // Control Flow
    IF, ELSE, WHILE, BREAK, CONTINUE, RETURN, END_OF_FILE,

    // Complex
    COMPLEX,
};

using Token = std::pair<TokenType, std::string>;

class Lexer {
    private:
        std::string code;
        int current_char;
        std::string::iterator it;
        const std::unordered_map<std::string, TokenType> table = {
            {"def", TokenType::DEF}      , {"extern", TokenType::EXTERN},
            {"let", TokenType::LET}      , {"if", TokenType::IF},
            {"else", TokenType::ELSE}    , {"while", TokenType::WHILE},
            {"break", TokenType::BREAK}  , {"continue", TokenType::CONTINUE},
            {"return", TokenType::RETURN}, {"true", TokenType::BOOL},
            {"false", TokenType::BOOL}   , {"module", TokenType::MODULE}
        };
    public:
        Lexer(const std::string &code);
        Token get_token();
        ~Lexer() = default;
};

#endif