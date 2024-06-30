#pragma once
#include <exception>
#include <sstream>
class InvalidTypeException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    InvalidTypeException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Invalid Type]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};