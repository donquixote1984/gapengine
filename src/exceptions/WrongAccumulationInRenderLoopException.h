#pragma once
#include <exception>
#include <sstream>
class WrongAccumulationInRenderLoopException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    WrongAccumulationInRenderLoopException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Wrong Accumulation]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};