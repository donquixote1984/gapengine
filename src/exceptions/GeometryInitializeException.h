#pragma once
#include <exception>
#include <string>
#include <sstream>

class GeometryInitializeException: public std::exception
{
private:
    const char * msg;
    mutable std::string whatBuffer;
public:
    GeometryInitializeException(const char * msg):msg(msg) {}
    const char * what() const throw() {
        std::ostringstream ss;
        ss << "[Geometry Init Error]" << msg;
        whatBuffer = ss.str();
        return whatBuffer.c_str();
    }
};