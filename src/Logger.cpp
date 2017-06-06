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
#include <chrono>
#include "EvoBeeModel.h"
#include "Environment.h"
#include "Pollinator.h"
#include "ModelParams.h"
#include "Logger.h"

namespace fs = std::experimental::filesystem;
using json = nlohmann::json;


Logger::Logger(EvoBeeModel* pModel) :
    m_strConfigFileSuffix {"-config.json"},
    m_strMainLogFileSuffix {"-log.txt"},
    m_pModel(pModel)
{
    assert(ModelParams::initialised());

    m_pEnv = &(m_pModel->getEnv());

    m_LogDir = ModelParams::getLogDir();

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

        // get timestamp to add to filename prefix
        std::stringstream ts;
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        ts << std::put_time(std::localtime(&now_c), "-%F-%H-%M-%S");
        m_strFilePrefix = ModelParams::getLogRunName() + ts.str();

        // set name of main log file
        std::string filename {m_strFilePrefix + m_strMainLogFileSuffix};
        m_MainLogFile = m_LogDir / filename;
    }
    catch (std::exception& e)
    {
        std::cerr << "Unable to set up Logger:" << e.what() << std::endl;
        exit(1);
    }

}


Logger::~Logger()
{
    ///@todo close all open log files?
}


void Logger::logExptSetup()
{
    std::string filename {m_strFilePrefix + m_strConfigFileSuffix};
    fs::path fullname {m_LogDir / filename};
    std::ofstream ofs {fullname};
    if (!ofs)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << fullname << " for writing";
        throw std::runtime_error(msg.str());
    }
    ofs << std::setw(4) << ModelParams::getJson() << std::endl;
}


void Logger::update()
{
    // open log file for appending
    std::ofstream ofs {m_MainLogFile, std::ofstream::app};
    if (!ofs)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << m_MainLogFile << " for writing";
        throw std::runtime_error(msg.str());
    }

    // log current step number of model
    //ofs << "==> " << m_pModel->getStepNumber() << std::endl;
    auto step = m_pModel->getStepNumber();

    // log data on all pollinators
    auto pollinators = m_pEnv->getAllPollinators();
    for (auto p : pollinators)
    {
        ofs << step << "," << p->getStateString() << std::endl;
    }

    // log flowering plants - and other stuff?
    ///@todo
}