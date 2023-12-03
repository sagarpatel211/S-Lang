#ifndef SLANG_HPP
#define SLANG_HPP
#pragma once

#include <iostream>
#include <fstream>
#include "debug_stream.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "codegen.hpp"

class Slang {
    private:
        std::string code;
        Lexer lexer;
    public:
        Slang(const std::string &code);
        void printIR() const;
        void write_to_file(const std::string &filename) const;
        ~Slang() = default;
};

#endif