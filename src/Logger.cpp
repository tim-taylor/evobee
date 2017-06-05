/**
 * @file
 *
 * Implementation of the Logger class
 */

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include "ModelParams.h"
#include "Logger.h"

namespace fs = std::experimental::filesystem;
using json = nlohmann::json;

Logger::Logger()
{
    assert(ModelParams::initialised());

    // read ModelParams to get the directory and filename suffix for logging output
    m_LogDir = ModelParams::getLogDir();
    m_strLogFilePrefix = ModelParams::getLogRunName();

    try
    {
        // check that the directory exists (if not, make it)
        if (fs::exists(m_LogDir)) 
        {
            if (!fs::is_directory(m_LogDir))
            {
                // file exists but it is not a directory!
                std::stringstream msg;
                msg << "Cannot use specified log directory " << m_LogDir << ": exists but not a directory";
                throw std::runtime_error(msg.str());
            }
        }
        else
        {
            fs::create_directories(m_LogDir);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Unable to set up Logger:" << e.what() << std::endl;
        exit(1);
    }

}


Logger::~Logger()
{
    ///@todo close all open log files
}


void Logger::logExptSetup()
{
    ///@todo should check if output file already exists?
    std::string filename {m_strLogFilePrefix + "-config.json"};
    fs::path fullname {m_LogDir / filename};
    std::ofstream ofs {fullname};
    if (!ofs)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << fullname << " for writing";
        throw std::runtime_error(msg.str());
    }
    ofs << std::setw(4) << ModelParams::getJson() << std::endl;
    ofs.close();
}


/**
@todo
*/
void Logger::update()
{

}