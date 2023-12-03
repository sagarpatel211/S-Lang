#include "lexer.hpp"
#include "exceptions.hpp"
#include "debug_stream.hpp"

Lexer::Lexer(const std::string &code) : code(std::move(code)), current_char(' '), it(this->code.begin()) {
    debug << "[DEBUG] Lexer initialized." << std::endl;
}

bool is_operator(const int& op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || 
        op == '%' || op == '>' || op == '<' || op == '=' || op == '!';
}

bool isValidNextChar(char op, char nextChar) {
    switch (op) {
        case '<':
        case '>':
        case '=':
        case '!':
            return nextChar == '\0' || nextChar == '=';
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return nextChar == '\0';
        default:
            return false;
    }
}

Token Lexer::get_token() {
    try {
        // Skip whitespaces 
        while (std::isspace(current_char)) {
            current_char = *(it++);
        }

        // Handle single-lined comments
        if (current_char == '#') {
            debug << "[DEBUG] Comment: Ignoring line." << std::endl;
            do {
                current_char = *(it++);
            } while (current_char != '\0' && current_char != '\n' && current_char != '\r');
            if (current_char != '\0') {
                return get_token();
            }
        }

        // Handle Integer and Floats
        if (std::isdigit(current_char) || current_char == '.') {
            std::string num_str;
            bool decimal_found = false; // Flag for only allowing 1 decimal point
            do {
                if (current_char == '.') {
                    if (decimal_found) {
                        throw invalid_number_error("More than one decimal point in a number.");
                    }
                    decimal_found = true;
                }
                num_str += current_char;
                current_char = *(it++);
            } while (std::isdigit(current_char) || current_char == '.');

            if (decimal_found) {
                debug << "[DEBUG] Float: " << num_str << std::endl;
                return {TokenType::FLOAT, num_str};
            }
            debug << "[DEBUG] Integer: " << num_str << std::endl;
            return {TokenType::INT, num_str};
        }

        // Handles Identifiers
        std::string identifier = "";
        if (std::isalpha(current_char)) {
            identifier = current_char;
            while (std::isalnum(current_char = *(it++)) || current_char == '_') {
                identifier += current_char;
            }
            TokenType kind = TokenType::IDENTIFIER;
            if (table.find(identifier) != table.end()) {
                kind = table.at(identifier);
            }
            debug << "[DEBUG] Identifier: " << identifier << std::endl;
            return {kind, identifier};
        }

        // Handles End of File
        if (current_char == '\0') {
            debug << "[DEBUG] End of File." << std::endl;
            return {TokenType::END_OF_FILE, ""};
        }

        // Handles Operators
        std::string op = "";
        int prev_char = current_char;
        current_char = *(it++);

        if (!is_operator(prev_char)) {
            debug << "[DEBUG] Complex: " << prev_char << std::endl;
            return {TokenType::COMPLEX, std::string(1, prev_char)};
        }

        op = prev_char;
        while (isValidNextChar(op.back(), current_char)) {
            op += current_char;
            current_char = *(it++);
        }
        debug << "[DEBUG] Operator: " << op << std::endl;
        return {TokenType::OPERATOR, op};

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "ERROR: Unknown error occurred!" << std::endl;
        exit(1);
    }
}
