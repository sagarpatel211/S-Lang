/**
 * @file debug_stream.hpp
 * @brief Debug Stream Utility for Conditional Debug Output
 *
 * Provides the DebugStream class for conditional debug messages, allowing messages
 * to be output based on the 'debug_mode' flag. It's useful in cases where verbose
 * logging is needed during development or debugging.
 *
 * @author Sagar Patel
 * @date 12-3-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef DEBUG_STREAM_HPP
#define DEBUG_STREAM_HPP
#pragma once

#include <iostream>

/**
 * @brief A debug class for conditional debug output.
 *
 * DebugStream provides a way to output debug information conditionally based on a CLI
 * flag. It overloads the `<<` operator to enable usage similar to standard output
 * streams. DebugStream checks the 'debug_mode' flag before outputting any message. If
 * 'debug_mode' is false, it does nothing, which will silence the debug output. Otherwise,
 * it outputs to the console.
 */
class DebugStream;

// Global debug mode flag
extern bool debug_mode;

// Global instance of DebugStream
extern DebugStream debug;

class DebugStream {
  public:
    /**
     * @brief Overloaded `<<` operator for general types.
     *
     * This template function allows DebugStream to accept various types of data,
     * by using std::cout. Only outputs the message if 'debug_mode' is true.
     *
     * @tparam T The type of the message being output.
     * @param msg The message to output.
     * @return Returns a reference to itself, allowing chaining of output operations.
     */
    template <typename T>
    DebugStream& operator<<(const T& msg) {
        if (debug_mode)
            std::cout << msg;
        return *this;
    }

    /**
     * @brief Overload `<<` operator for std::ostream manipulators.
     *
     * Allows DebugStream to handle stream manipulators like std::endl, std::flush, etc.
     * It checks 'debug_mode' and applies the manipulator to std::cout if true.
     *
     * @param manip The manipulator function.
     * @return Returns a reference to itself, allowing chaining of manipulator calls.
     */
    typedef std::ostream& (*StreamManipulator)(std::ostream&);
    DebugStream& operator<<(StreamManipulator manip) {
        if (debug_mode)
            manip(std::cout);
        return *this;
    }
};

#endif