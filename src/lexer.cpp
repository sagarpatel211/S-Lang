#include "lexer.hpp"

/**
 * Constructor for Lexer class
 */
Lexer::Lexer(const std::string& code) : code(std::move(code)), current_char(' '), it(this->code.begin()) {
    debug << "[DEBUG] Lexer initialized." << std::endl;
}

/**
 * Checks if the given character is an operator
 */
bool is_operator(const int& op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '>' || op == '<' || op == '=' ||
           op == '!';
}

/**
 * Checks if the next character is valid for the current operator
 * For example, '<' can be followed by '=' or '\0' but '*' can only be followed by '\0'
 */
bool is_valid_next_char(char op, char next_char) {
    switch (op) {
    case '<':
    case '>':
    case '=':
    case '!':
        return next_char == '\0' || next_char == '=';
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
        return next_char == '\0';
    default:
        return false;
    }
}

/**
 * Checks if the given string is a keyword, mainly used for comments
 * F
 */
bool Lexer::is_keyword(const std::string& keyword) {
    int tmp = this->current_char;
    for (const auto& ch : keyword) {
        if ((tmp != ch) || (tmp == '\0') || (tmp == '\n') || (tmp == '\r')) {
            return false;
        }
        tmp = *(it++);
    }
    current_char = tmp;
    return true;
}

Token Lexer::get_token() {
    try {
        // Skip any whitespace characters (like spaces, tabs, newline) to find the start of the next token.
        while (std::isspace(current_char)) {
            current_char = *(it++);
        }

        // If the current token starts with "Cancelled", it's a single-line comment. Skip the entire line.
        if (is_keyword("Cancelled")) {
            debug << "[DEBUG] Comment: Ignoring line." << std::endl;
            do {
                current_char = *(it++);
            } while (current_char != '\0' && current_char != '\n' && current_char != '\r');
            if (current_char != '\0') {
                return get_token();
            }
        }

        // If the current token starts with "Blocked", it's a multi-line comment. Skip text until "Unblocked" is found.
        if (is_keyword("Blocked")) {
            debug << "[DEBUG] Comment: Ignoring block." << std::endl;
            do {
                current_char = *(it++);
            } while (current_char != '\0' && (!is_keyword("Unblocked")));
            if (current_char != '\0') {
                return get_token(); // After skipping, get the next token if not at end of file
            }
        }

        // Handle character literals, which are enclosed in single quotes.
        if (current_char == '\'') {
            current_char = *(it++);
            debug << "[DEBUG] Char: " << std::string(1, current_char) << std::endl;
            char char_value = current_char;
            if (*(it++) != '\'') {
                throw invalid_literal_error("Invalid char token: " + std::string(1, current_char));
            }
            current_char = *((it++)++);
            debug << "[DEBUG] Char: " << char_value << std::endl;
            std::string tmp_str(1, char_value);
            return {TokenType::CHAR, tmp_str};
            // Handle string literals, which are enclosed in double quotes.
        } else if (current_char == '\"') {
            std::string str_val = "";
            current_char = *(it++);
            while (current_char != '\"') {
                if (current_char == '\0') {
                    throw invalid_literal_error("Invalid string token: " + str_val);
                }
                str_val += current_char;
                current_char = *(it++);
            }
            current_char = *(it++);
            debug << "[DEBUG] String: " << str_val << std::endl;
            return {TokenType::STRING, str_val};
        }

        // Handle numeric literals, both integer and floating-point.
        if (std::isdigit(current_char) || current_char == '.') {
            std::string num_str;
            bool decimal_found = false; // Flag for only allowing 1 decimal point
            do {
                if (current_char == '\0') {
                    throw invalid_literal_error("Invalid number token: " + num_str);
                }
                if (current_char == '.') {
                    if (decimal_found) { // Ensure there's only one decimal point.
                        throw invalid_literal_error("More than one decimal point in number: " + num_str);
                    }
                    decimal_found = true;
                }
                num_str += current_char;
                current_char = *(it++);
            } while (std::isdigit(current_char) || current_char == '.');

            // Determine if the number is a float or an integer.
            if (decimal_found) {
                debug << "[DEBUG] Float: " << num_str << std::endl;
                return {TokenType::FLOAT, num_str};
            }
            debug << "[DEBUG] Integer: " << num_str << std::endl;
            return {TokenType::INT, num_str};
        }

        // Handle identifiers (eg. keywords + variable names), which start with an alphabet and may contain alphanumeric
        // characters or underscores.
        std::string identifier = "";
        if (std::isalpha(current_char)) {
            identifier = current_char;
            while (std::isalnum(current_char = *(it++)) || current_char == '_' || current_char == '?') {
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

        // Check if the previous character is a valid operator
        if (!is_operator(prev_char)) {
            std::string s(1, char(prev_char));
            debug << "[DEBUG] Complex: " << s << std::endl;
            return {TokenType::COMPLEX, std::string(1, prev_char)};
        }

        op = prev_char;
        // Build the complete operator token if necessary (eg. '++', '+=', '==', etc.)
        while (is_valid_next_char(op.back(), current_char)) {
            op += current_char;
            current_char = *(it++);
        }
        debug << "[DEBUG] Operator: " << op << std::endl;
        return {TokenType::OPERATOR, op};

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error occurred!" << std::endl;
        exit(1);
    }
}
