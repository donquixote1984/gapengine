#pragma once
#include <exception>
#include <string>
#include <sstream>

class FrameBufferInitializeException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    FrameBufferInitializeException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Frame Buffer Init Error]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};