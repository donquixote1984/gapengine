#pragma once
#include <exception>
#include <string>
#include <sstream>

class ReaderException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    ReaderException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Reader Exception]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};