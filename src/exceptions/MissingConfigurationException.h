#pragma once
#include <exception>
#include <string>
#include <sstream>

class MissingConfigurationException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    MissingConfigurationException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Missing Configuration]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};