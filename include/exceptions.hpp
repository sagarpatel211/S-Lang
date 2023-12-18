/**
 * @file exceptions.hpp
 * @brief Custom Exception Handling for a C++ Application
 *
 * This file defines custom exception classes for handling specific error conditions
 * in my language. The exceptions are derived from the `std::exception` class.
 * Each exception class overrides the `what()` method to return a custom error message.
 *
 * @author Sagar Patel
 * @date 12-17-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#pragma once

#include <iostream>

/**
 * @brief Exception for errors in processing code files.
 *
 * This exception is thrown when a file cannot be processed due to issues such as
 * file not found, read permissions, or corrupted content. Since the file cannot
 * be processed, the program cannot continue and must exit.
 *
 * @note Inherits from std::exception.
 */
class process_file_error : public std::exception {
  private:
    std::string message;
  public:
    process_file_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Exception for invalid literal input.
 *
 * This exception is used to indicate errors in literal input, specifically
 * when a number has too many decimal points or other formatting issues, a string
 * is not properly written, or a character is not properly written.
 *
 * @note Inherits from std::exception.
 */
class invalid_literal_error : public std::exception {
  private:
    std::string message;
  public:
    invalid_literal_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Exception for invalid syntax.
 *
 * This exception is used to indicate errors in syntax, such as missing
 * parentheses or brackets, or invalid characters and such.
 *
 * @note Inherits from std::exception.
 */
class parse_logic_error : public std::exception {
  private:
    std::string message;
  public:
    parse_logic_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Exception for errors in code generation.
 *
 * This exception is used to indicate errors in code generation, such as
 * invalid function calls or invalid variable declarations.
 *
 * @note Inherits from std::exception.
 */
class codegen_error : public std::exception {
  private:
    std::string message;
  public:
    codegen_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

#endif