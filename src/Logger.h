#pragma once
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include <sstream>

class Logger
{
private:
    Logger();
    Logger(const Logger&) = delete;
public:
    static Logger* GetLogger();
    void log(const std::string &str);
    void log(float * arr,  int count, std::string prefix = "");
};