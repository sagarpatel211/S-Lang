#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#pragma once

#include <iostream>


/* == Error for when code file cannot be processed == */
class process_file_error : public std::exception {
    private:
        const char* message;
    public:
        process_file_error(const char* msg) : message(msg) {}
        const char *what() const noexcept override {
            return message;
        }
};


/* == Error for when a number has too many decimal points == */
class invalid_number_error : public std::exception {
    private:
        const char* message;
    public:
        invalid_number_error(const char* msg) : message(msg) {}
        const char *what() const noexcept override {
            return message;
        }
};

#endif