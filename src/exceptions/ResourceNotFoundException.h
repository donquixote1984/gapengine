#pragma once
#include <exception>
#include <string>
#include <sstream>

class ResourceNotFoundException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    ResourceNotFoundException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Resource Not Found]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};