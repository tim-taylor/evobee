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

    if (ModelParams::logging())
    {
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
}


Logger::~Logger()
{
    ///@todo close all open log files?
}


void Logger::logExptSetup()
{
    assert(ModelParams::logging());
    
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


void Logger::logPollinatorsFull()
{
    std::ofstream ofs = openLogFile();

    auto gen = m_pModel->getGenNumber();
    auto step = m_pModel->getStepNumber();
    auto& pollinators = m_pEnv->getAllPollinators();

    for (auto p : pollinators)
    {
        ofs << "P," << gen << "," << step << "," << p->getStateString() << std::endl;
    }
}


void Logger::logFlowersFull()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                ofs << "F," << gen << "," << plant.getId() << "," << plant.getSpeciesId()
                    << "," << "<Logger::logFlowersFull incomplete>" << std::endl;

                /*
                if (plant.pollinated())
                {
                    const std::vector<Flower>& flowers = plant.getFlowers();
                    for (const Flower& flower : flowers)
                    {
                        const PollenVector& stigmaPollen = flower.getStigmaPollen();
                        for (const Pollen& pollen : stigmaPollen)
                        {
                            ///@todo - at some point we actually need to log something!!
                        }
                    }
                }
                */
            }
        }
    }
}


void Logger::logFlowersSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber(); 
    std::vector<Patch>& patches = m_pEnv->getPatches();
    std::map<unsigned int, unsigned int> speciesCounts;

    const std::map<unsigned int, std::string>& speciesInfoMap = FloweringPlant::getSpeciesMap();
    for (auto& speciesInfo : speciesInfoMap)
    {
        speciesCounts[speciesInfo.first] = 0;
    }

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                speciesCounts[plant.getSpeciesId()]++;
            }
        }
    }

    for (auto& countInfo : speciesCounts)
    {
        ofs << "f," << gen << countInfo.first << "," << speciesInfoMap.at(countInfo.first)
            << "," << countInfo.second << std::endl;
    }
}


// private helper method to open the log file for appending
// (the compiler should use the move assignment operator to efficiently return the ofstream object)
std::ofstream Logger::openLogFile()
{
    assert(ModelParams::logging());

    // open log file for appending
    std::ofstream ofs {m_MainLogFile, std::ofstream::app};
    if (!ofs)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << m_MainLogFile << " for writing";
        throw std::runtime_error(msg.str());
    }

    return ofs; // the compiler should use the move assignment operator here
}
