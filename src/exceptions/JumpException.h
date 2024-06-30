#pragma once
#include <exception>
#include <sstream>
class JumpException : public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    JumpException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Jump Out for Shadow Debugging]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};