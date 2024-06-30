#pragma once
#include <exception>
#include <string>
#include <sstream>

class ErrorContentException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    ErrorContentException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Content Error]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};