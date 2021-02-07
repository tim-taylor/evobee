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
#include <iomanip>
#include <string>
#include <tuple>
#include <experimental/filesystem>
#include <chrono>
#include <utility>
#include "evobeeConfig.h"
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
    m_strRunInfoFileSuffix {"-info.txt"},
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
            // and add a random number to ensure it is really unique
            std::uniform_int_distribution<int> dist(0, 999999);
            int uid = dist(EvoBeeModel::m_sRngEngine);
            ts << "-" << std::right << std::setfill('0') << std::setw(6) << uid;
            // and add all of this to the filename prefix
            m_strFilePrefix = ModelParams::getLogRunName() + ts.str();

            // set name of main log file
            m_strMainLogFilename = m_strFilePrefix + m_strMainLogFileSuffix;
            m_MainLogFilePath = m_LogDir / m_strMainLogFilename;

            // set name of expt config file log
            m_strConfigFilename = m_strFilePrefix + m_strConfigFileSuffix;
            m_ConfigFilePath = m_LogDir / m_strConfigFilename;

            // set name of run info file
            m_strRunInfoFilename = m_strFilePrefix + m_strRunInfoFileSuffix;
            m_RunInfoFilePath = m_LogDir / m_strRunInfoFilename;
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
}


void Logger::logExptSetup()
{
    assert(ModelParams::logging());

    // output a copy of the experiment's JSON config file
    std::ofstream ofs1 {m_ConfigFilePath};
    if (!ofs1)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << m_ConfigFilePath << " for writing";
        throw std::runtime_error(msg.str());
    }
    ofs1 << std::setw(4) << ModelParams::getJson() << std::endl;

    // output general run info (e.g. program version number, etc)
    std::ofstream ofs2 {m_RunInfoFilePath};
    if (!ofs2)
    {
        std::stringstream msg;
        msg << "Unable to open run info file " << m_RunInfoFilePath << " for writing";
        throw std::runtime_error(msg.str());
    }
    ofs2 << "Git branch = " << evobee_GIT_BRANCH << std::endl
         << "Git commit hash = " << evobee_GIT_COMMIT_HASH << std::endl
         << "Program version = " << evobee_VERSION_MAJOR << "." << evobee_VERSION_MINOR
         << "." << evobee_VERSION_PATCH << "." << evobee_VERSION_TWEAK << std::endl;
}


// Log full details of pollinators at regular intervals within a foraging phase
//
// This logging is designated by log-flags="Q" in the JSON config file
//
void Logger::logPollinatorsIntraPhaseFull()
{
    std::ofstream ofs = openLogFile();

    auto gen = m_pModel->getGenNumber();
    auto step = m_pModel->getStepNumber();
    auto& pollinators = m_pEnv->getAllPollinators();

    for (auto p : pollinators)
    {
        ofs << "Q," << gen << "," << step << "," << p->getStateString() << std::endl;
    }
}


// Log full details of pollinators at the end of each foraging phase
//
// This logging is designated by log-flags="P" in the JSON config file
//
void Logger::logPollinatorsInterPhaseFull()
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


// Log summary details of pollinators at the end of each foraging phase
//
// This logging is designated by log-flags="p" in the JSON config file
//
void Logger::logPollinatorsInterPhaseSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    auto& pollinators = m_pEnv->getAllPollinators();

    for (auto pPol : pollinators)
    {
        ofs << "p," << gen << "," << pPol->getId();

        auto& perfMap = pPol->getPerformanceInfoMap();
        for (auto& perfInfo : perfMap)
        {
            ofs << "," << perfInfo.first << "," << perfInfo.second.numLandings
                << "," << perfInfo.second.numPollinations << ","
                << pPol->getNumPollenGrainsInStore(perfInfo.first);
        }

        ofs << std::endl;
    }
}


// Log full details of flowers at the end of each foraging phase
//
// This logging is designated by log-flags="F" in the JSON config file
//
void Logger::logFlowersInterPhaseFull()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    std::map<MarkerPoint, std::pair<int, unsigned int>> pollenSourceMpMap;

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            //const iPos& pos = patch.getPosition();

            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                const fPos& pos = plant.getPosition();

                ofs << "F," << gen << "," << plant.getId() << "," << plant.getSpeciesId()
                    << "," << pos << "," << patch.getLocalityId();

                if (true) //plant.pollinated())
                {
                    const std::vector<Flower>& flowers = plant.getFlowers();
                    for (const Flower& flower : flowers)
                    {
                        pollenSourceMpMap.clear();

                        MarkerPoint thisLambda = flower.getCharacteristicWavelength();
                        ofs << ",:," << flower.getId() << "," << (flower.pollinated() ? "P" : "N")
                            << "," << thisLambda << ",~,";

                        const PollenVector& stigmaPollen = flower.getStigmaPollen();
                        for (const Pollen& pollen : stigmaPollen)
                        {
                            const Flower* pSourceFlower = pollen.pSource;
                            MarkerPoint sourceLambda = pSourceFlower->getCharacteristicWavelength();
                            auto it = pollenSourceMpMap.find(sourceLambda);
                            if (it == pollenSourceMpMap.end()) {
                                pollenSourceMpMap.insert(std::make_pair(sourceLambda,
                                                            std::make_pair(1, pSourceFlower->getId())));
                            }
                            else {
                                it->second.first++;
                            }
                        }

                        for (auto& info : pollenSourceMpMap) {
                            // NB the final item in the output triplet is the unique idea of the pollen source
                            // flower, but this only makes sense when there is just a single pollen grain of
                            // a given species. If there is more than one pollen grain of the species present,
                            // they may have come from various different source flowers.
                            ofs << info.first << "," << info.second.first << "," << info.second.second << ",";
                        }

                        ofs << "~";
                    }
                }

                ofs << std::endl;
            }
        }
    }
}


// Log summary details of flowers at the end of each foraging phase
//
// This logging is designated by log-flags="f" in the JSON config file
//
void Logger::logFlowersInterPhaseSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    // create a map of species ID to counts of number of plants and number of pollinated plants
    std::map<unsigned int, std::pair<unsigned int,unsigned int>> speciesCounts;

    const std::map<unsigned int, std::string>& speciesInfoMap = FloweringPlant::getSpeciesMap();
    for (auto& speciesInfo : speciesInfoMap)
    {
        speciesCounts[speciesInfo.first] = std::make_pair(0,0);
    }

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                speciesCounts[plant.getSpeciesId()].first++;
                if (plant.pollinated())
                {
                    speciesCounts[plant.getSpeciesId()].second++;
                }
            }
        }
    }

    for (auto& countInfo : speciesCounts)
    {
        ofs << "f," << gen << "," << m_pModel->getStepNumber() << ","
            << countInfo.first << "," << speciesInfoMap.at(countInfo.first)
            << "," << countInfo.second.first << "," << countInfo.second.second << std::endl;
    }
}


// Log full details of flowers at regular intervals within a foraging phase
//
// This logging is designated by log-flags="G" in the JSON config file
//
void Logger::logFlowersIntraPhaseFull()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    auto step = m_pModel->getStepNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                Flower* pFlower = plant.getFlower(0);
                ofs << "G," << gen << "," << step << "," << pFlower->getStateString() << std::endl;
            }
        }
    }

}


// Log summary details of flowers at regular intervals within a foraging phase
//
// This logging is designated by log-flags="g" in the JSON config file
//
void Logger::logFlowersIntraPhaseSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    auto step = m_pModel->getStepNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    // create a map of species ID to counts of number of plants and number of pollinated plants
    std::map<unsigned int, std::pair<unsigned int,unsigned int>> speciesCounts;

    const std::map<unsigned int, std::string>& speciesInfoMap = FloweringPlant::getSpeciesMap();
    for (auto& speciesInfo : speciesInfoMap)
    {
        speciesCounts[speciesInfo.first] = std::make_pair(0,0);
    }

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                speciesCounts[plant.getSpeciesId()].first++;
                if (plant.pollinated())
                {
                    speciesCounts[plant.getSpeciesId()].second++;
                }
            }
        }
    }

    for (auto& countInfo : speciesCounts)
    {
        ofs << "g," << gen << "," << step << "," << countInfo.first << "," << speciesInfoMap.at(countInfo.first)
            << "," << countInfo.second.first << "," << countInfo.second.second << std::endl;
    }
}


// Log summary details of flower marker points at the end of each foraging phase
//
// This logging is designated by log-flags="m" in the JSON config file
//
void Logger::logFlowerMPsInterPhaseSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    // create a map of marker points to counts of number of plants
    // the entries in the tuple represent the following:
    // 0. count of number of flowers with this ID
    // 1. count of number of pollinated flowers with this ID
    // 2. count of number of flowers with this ID in communal (non-refuge) areas of environment
    // 3. count of number of pollinated flowers with this ID in communal (non-refuge) areas of environment
    std::map<Wavelength,
             std::tuple<unsigned int,unsigned int,unsigned int,unsigned int>> mpCounts;

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            bool bCommunal = !(patch.refuge());
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                unsigned int pol = plant.pollinated() ? 1 : 0;
                unsigned int communal_num = bCommunal ? 1 : 0;
                unsigned int communal_pol = bCommunal ? pol : 0;
                Wavelength lambda = plant.getFlowerCharacteristicWavelength();
                auto it = mpCounts.find(lambda);
                if (it == mpCounts.end()) {
                    mpCounts.insert(std::make_pair(lambda, std::make_tuple(1, pol, communal_num, communal_pol)));
                }
                else {
                    std::get<0>(it->second)++;
                    std::get<1>(it->second) += pol;
                    std::get<2>(it->second) += communal_num;
                    std::get<3>(it->second) += communal_pol;
                }
            }
        }
    }

    for (auto& countInfo : mpCounts)
    {
        ofs << "m," << gen << "," << m_pModel->getStepNumber() << "," << countInfo.first
            << "," << std::get<0>(countInfo.second) << "," << std::get<1>(countInfo.second)
            << "," << std::get<2>(countInfo.second) << "," << std::get<3>(countInfo.second)
            << std::endl;
    }
}


// Log summary details of flowers at the end of each foraging phase, aggregated by
// the VisualStimulusInfo.id associated with each flower
//
// This logging is designated by log-flags="n" in the JSON config file
//
void Logger::logFlowerInfoInterPhaseSummary()
{
    std::ofstream ofs = openLogFile();
    auto gen = m_pModel->getGenNumber();
    std::vector<Patch>& patches = m_pEnv->getPatches();

    // create a map of vsi.ids to counts of number of plants
    // the entries in the tuple represent the following:
    // 0. count of number of flowers with this ID
    // 1. count of number of pollinated flowers with this ID
    // 2. count of number of flowers with this ID in communal (non-refuge) areas of environment
    // 3. count of number of pollinated flowers with this ID in communal (non-refuge) areas of environment
    // 4. the auxiliary ID associated with this flower ID (as defined in input data)
    // 5. the characteristic (dominant) wavelength associated with this flower ID
    // 6. count of number of landings by pollinators on flowers with this ID
    std::map<int,
             std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, int, Wavelength, unsigned int>> mpCounts;

    for (Patch& patch : patches)
    {
        if (patch.hasFloweringPlants())
        {
            bool bCommunal = !(patch.refuge());
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                Flower* pFlower = plant.getFlower(); // assuming just one flower per plant
                unsigned int pol = plant.pollinated() ? 1 : 0;
                unsigned int communal_num = bCommunal ? 1 : 0;
                unsigned int communal_pol = bCommunal ? pol : 0;
                unsigned int landings = pFlower->getPollinatorLandingCount();
                const ReflectanceInfo& reflectance = plant.getFlowerReflectanceInfo();
                const VisualStimulusInfo* pVSI = reflectance.getVisDataPtr();
                if (pVSI == nullptr) {
                    throw std::runtime_error("Retrieved null pointer for plant's visual data in Logger::logFlowerInfoInterPhaseSummary. Aborting!");
                }
                int id = pVSI->id;
                auto it = mpCounts.find(id);
                if (it == mpCounts.end()) {
                    Wavelength lambda = plant.getFlowerCharacteristicWavelength();
                    mpCounts.insert(std::make_pair(id,
                        std::make_tuple(1, pol, communal_num, communal_pol, pVSI->aux_id, lambda, landings)));
                }
                else {
                    std::get<0>(it->second)++;
                    std::get<1>(it->second) += pol;
                    std::get<2>(it->second) += communal_num;
                    std::get<3>(it->second) += communal_pol;
                    std::get<6>(it->second) += landings;
                }
            }
        }
    }

    for (auto& countInfo : mpCounts)
    {
        ofs << "n," << gen << "," << m_pModel->getStepNumber() << "," << countInfo.first
            << "," << std::get<0>(countInfo.second) << "," << std::get<1>(countInfo.second)
            << "," << std::get<2>(countInfo.second) << "," << std::get<3>(countInfo.second)
            << "," << std::get<4>(countInfo.second) << "," << std::get<5>(countInfo.second)
            << "," << std::get<6>(countInfo.second)
            << std::endl;
    }
}


// private helper method to open the log file for appending
// (the compiler should use the move assignment operator to efficiently return the ofstream object)
std::ofstream Logger::openLogFile()
{
    assert(ModelParams::logging());

    // open log file for appending
    std::ofstream ofs {m_MainLogFilePath, std::ofstream::app};
    if (!ofs)
    {
        std::stringstream msg;
        msg << "Unable to open log file " << m_MainLogFilePath << " for writing";
        throw std::runtime_error(msg.str());
    }

    return ofs; // the compiler should use the move assignment operator here
}


// if ModelParams::m_strLogFinalDir has been set, then we need to transfer all log files
// from this run from m_strLogDir to m_strLogFinalDir at the end of the run
void Logger::transferFilesToFinalDir()
{
    const std::string& strLogFinalDir = ModelParams::getLogFinalDir();

    if (!strLogFinalDir.empty())
    {
        fs::path finalDirPath {strLogFinalDir};

        try
        {
            if (fs::exists(finalDirPath))
            {
                if (!fs::is_directory(finalDirPath))
                {
                    // file exists but it is not a directory!
                    std::stringstream msg;
                    msg << "Cannot use specified final log directory " << finalDirPath
                        << ": exists but not a directory";
                    throw std::runtime_error(msg.str());
                }
            }
            else
            {
                fs::create_directories(finalDirPath);
            }

            fs::path mainLogFileFinalPath = finalDirPath / m_strMainLogFilename;
            fs::path configFileFinalPath  = finalDirPath / m_strConfigFilename;
            fs::path runInfoFileFinalPath = finalDirPath / m_strRunInfoFilename;

            // NB to move the files from their current location to their final destination,
            // we first copy them to the new location, then delete the old files. We do this
            // in two steps rather than using the single fs::rename method, because the latter
            // is susceptible to problems if the two directories are mounted on different
            // volumes; for more info see, e.g.,
            // https://stackoverflow.com/questions/24209886/invalid-cross-device-link-error-with-boost-filesystem

            fs::copy(m_MainLogFilePath, mainLogFileFinalPath);
            fs::copy(m_ConfigFilePath,  configFileFinalPath);
            fs::copy(m_RunInfoFilePath, runInfoFileFinalPath);

            fs::remove(m_MainLogFilePath);
            fs::remove(m_ConfigFilePath);
            fs::remove(m_RunInfoFilePath);
        }
        catch (std::exception& e)
        {
            std::cerr << "Unable to move log files to final destination directory: " << e.what() << std::endl;
        }
    }
}
