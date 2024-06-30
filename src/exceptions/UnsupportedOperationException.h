#pragma once
#include <exception>
#include <string>
#include <sstream>

class UnsupportedOperationException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    UnsupportedOperationException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Unsupported Operation]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};