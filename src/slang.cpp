#include "slang.hpp"

Slang::Slang(const std::string &code) : code(code), lexer(code) {}

void Slang::printIR() const {
    // TO DO
}

void Slang::write_to_file(const std::string &filename) const {
    std::ofstream out(filename);
    // TO DO
}