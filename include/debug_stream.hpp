#ifndef DEBUG_STREAM_HPP
#define DEBUG_STREAM_HPP
#pragma once

#include <iostream>

class DebugStream;
extern bool debug_mode;
extern DebugStream debug;

class DebugStream {
    public:
        template<typename T>
        DebugStream& operator<<(const T& msg) {
            if (debug_mode) std::cout << msg;
            return *this;
        }

        // Overload `<<` operator for std::ostream manipulators
        typedef std::ostream& (*StreamManipulator)(std::ostream&);
        DebugStream& operator<<(StreamManipulator manip) {
            if (debug_mode) manip(std::cout);
            return *this;
        }
};

#endif