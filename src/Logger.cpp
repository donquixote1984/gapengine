#include "Logger.h"

namespace logging  = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks    = boost::log::sinks;
Logger::Logger()
{
    logging::add_file_log(
        keywords::file_name     = "log_%N.log",
       // keywords::open_mode     = std::ios_base::app,
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::format        = "[%TimeStamp%]:<%Severity%> %Message%",
        keywords::auto_flush    = true 
    );
    logging::add_common_attributes();
}

Logger * Logger::GetLogger()
{
    static Logger logger;
    return &logger;
}
void Logger::log(const std::string &str)
{
    BOOST_LOG_TRIVIAL(debug) << str;
}

void Logger::log(float * arr, int count, std::string prefix)
{
    std::stringstream ss;
    ss << prefix << std::endl;
    for (int i = 0; i < count ; i++)
    {
        ss << std::to_string(arr[i]) << ", ";
    }
    log(ss.str());
}
