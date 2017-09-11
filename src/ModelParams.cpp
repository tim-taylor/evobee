/**
 * @file
 *
 * Implementation of the ModelParams class
 */

#include <exception>
#include "ModelParams.h"
#include "FloweringPlant.h"


// Instantiate all static members
bool   ModelParams::m_bVisualisation = true;
bool   ModelParams::m_bVisPollinatorTrails = false;
int    ModelParams::m_iVisUpdatePeriod = 1;
int    ModelParams::m_iVisDelayPerFrame = 0;
int    ModelParams::m_iLogUpdatePeriod = 1;
int    ModelParams::m_iEnvSizeX = 50;
int    ModelParams::m_iEnvSizeY = 50;
float  ModelParams::m_fEnvDefaultAmbientTemp = 20.0;
MarkerPoint ModelParams::m_EnvBackgroundReflectanceMP = 400;
bool   ModelParams::m_bReproGlobalDensityContrained = true;
float  ModelParams::m_fReproGlobalDensityMax = 0.2;
float  ModelParams::m_fMaxScreenFracH = 0.8;
float  ModelParams::m_fMaxScreenFracW = 0.8;
bool   ModelParams::m_bInitialised = false;
int    ModelParams::m_iSimTerminationNumGens = 10;
GenTerminationType ModelParams::m_GenTerminationType = GenTerminationType::NUM_SIM_STEPS;
int    ModelParams::m_iGenTerminationParam = -1;
float  ModelParams::m_fGenTerminationParam = -1.0;
bool   ModelParams::m_bLogging = true;
bool   ModelParams::m_bLogPollinatorsFull = false;
bool   ModelParams::m_bLogPollinatorsSummary = false;
bool   ModelParams::m_bLogFlowersFull = false;
bool   ModelParams::m_bLogFlowersSummary = false;
bool   ModelParams::m_bUseLogThreads = false;
bool   ModelParams::m_bVerbose = true;
unsigned int ModelParams::m_sNextFreePtdcId = 1;
std::string ModelParams::m_strLogDir {"output"};
std::string ModelParams::m_strLogFinalDir {""};
std::string ModelParams::m_strLogRunName {"run"};
std::string ModelParams::m_strRngSeed {""};

std::vector<HiveConfig> ModelParams::m_Hives;
std::vector<PlantTypeDistributionConfig> ModelParams::m_PlantDists;
std::vector<PlantTypeConfig> ModelParams::m_PlantTypes;
std::vector<PollinatorConfig> ModelParams::m_PollinatorConfigs;

nlohmann::json ModelParams::m_Json;


void ModelParams::setEnvSize(int x, int y)
{
    if (x > 0) m_iEnvSizeX = x;
    if (y > 0) m_iEnvSizeY = y;
}

void ModelParams::setEnvSizeX(int x)
{
    if (x > 0) m_iEnvSizeX = x;
}

void ModelParams::setEnvSizeY(int y)
{
    if (y > 0) m_iEnvSizeY = y;
}

void ModelParams::setVisualisation(bool vis)
{
    m_bVisualisation = vis;
}

/**
*/
void ModelParams::setMaxScreenFrac(float f)
{
    ModelParams::setMaxScreenFracW(f);
    ModelParams::setMaxScreenFracH(f);
}

void ModelParams::setMaxScreenFracW(float fw)
{
    if (fw < 0.1)
    {
        m_fMaxScreenFracW = 0.1;
        /// @todo... maybe output warning msg here and in next case?
    }
    else if (fw > 1.0)
    {
        m_fMaxScreenFracW = 1.0;
    }
    else
    {
        m_fMaxScreenFracW = fw;
    }
}

void ModelParams::setMaxScreenFracH(float fh)
{
    if (fh < 0.1)
    {
        m_fMaxScreenFracH = 0.1;
        /// @todo... maybe output warning msg here and in next case?
    }
    else if (fh > 1.0)
    {
        m_fMaxScreenFracH = 1.0;
    }
    else
    {
        m_fMaxScreenFracH = fh;
    }
}

void ModelParams::setEnvDefaultAmbientTemp(float temp)
{
    m_fEnvDefaultAmbientTemp = temp;
}

void ModelParams::setEnvBackgroundReflectanceMP(MarkerPoint mp)
{
    m_EnvBackgroundReflectanceMP = mp;
}

void ModelParams::setReproGlobalDensityConstrained(bool constrained)
{
    m_bReproGlobalDensityContrained = constrained;
}

void ModelParams::setReproGlobalDensityMax(float dmax)
{
    m_fReproGlobalDensityMax = dmax;
}

void ModelParams::setVisPollinatorTrails(bool show)
{
    m_bVisPollinatorTrails = show;
}

void ModelParams::setVisUpdatePeriod(int p)
{
    if (p > 0)
    {
        m_iVisUpdatePeriod = p;
    }
}

void ModelParams::setVisDelayPerFrame(int delayMs)
{
    if (delayMs >= 0)
    {
        m_iVisDelayPerFrame = delayMs;
    }
}

void ModelParams::setLogging(bool logging)
{
    m_bLogging = logging;
}

void ModelParams::setLogFlags(const std::string& flags)
{
    for (auto& flag : flags)
    {
        switch (flag)
        {
        case 'P':
        {
            m_bLogPollinatorsFull = true;
            break;
        }
        case 'p':
        {
            m_bLogPollinatorsSummary = true;
            break;
        }
        case 'F':
        {
            m_bLogFlowersFull = true;
            break;
        }
        case 'f':
        {
            m_bLogFlowersSummary = true;
            break;
        }
        default:
        {
            std::cerr << "Warning: Ignoring unknown log flag" << flag << std::endl;
        }
        }
    }
}

void ModelParams::setLogUpdatePeriod(int p)
{
    if (p > 0)
    {
        m_iLogUpdatePeriod = p;
    }
}

void ModelParams::setSimTerminationNumGens(int gens)
{
    m_iSimTerminationNumGens = gens;
}

void ModelParams::setGenTerminationType(const std::string& typestr)
{
    if (typestr == "num-sim-steps")
    {
        m_GenTerminationType = GenTerminationType::NUM_SIM_STEPS;
    }
    else if (typestr == "num-pollinator-steps")
    {
        m_GenTerminationType = GenTerminationType::NUM_POLLINATOR_STEPS;
    }
    else if (typestr == "pollinated-fraction")
    {
        m_GenTerminationType = GenTerminationType::POLLINATED_FRACTION;
    }
    else
    {
        m_GenTerminationType = GenTerminationType::NUM_SIM_STEPS;
        /// @todo... maybe output warning msg here and in next case?
    }
}

// implicit setting of int param
void ModelParams::setGenTerminationParam(int p)
{
    m_iGenTerminationParam = p;
}

// implicit setting of float param
void ModelParams::setGenTerminationParam(float p)
{
    m_fGenTerminationParam = p;
}

// explicit setting of int param
void ModelParams::setGenTerminationIntParam(int p)
{
    m_iGenTerminationParam = p;
}

// explicit setting of float param
void ModelParams::setGenTerminationFloatParam(float p)
{
    m_fGenTerminationParam = p;
}

void ModelParams::setRngSeedStr(const std::string& seed, bool bRewriteJsonEntry /*= false*/)
{
    m_strRngSeed = seed;

    if (bRewriteJsonEntry)
    {
        m_Json["SimulationParams"]["rng-seed"] = seed;
    }
}

void ModelParams::setLogDir(const std::string& dir)
{
    m_strLogDir = dir;
}

void ModelParams::setLogFinalDir(const std::string& dir)
{
    m_strLogFinalDir = dir;
}

void ModelParams::setLogRunName(const std::string& name)
{
    m_strLogRunName = name;
}

void ModelParams::setInitialised()
{
    m_bInitialised = true;
}

void ModelParams::addHiveConfig(HiveConfig& hc)
{
    m_Hives.push_back(hc);
}

void ModelParams::addPlantTypeDistributionConfig(PlantTypeDistributionConfig& ptdc)
{
    // first assign a unique ID to this PlantTypeDistributionConfig
    ptdc.id = m_sNextFreePtdcId++;

    // and add it to our list of all PlantTypeDistributionConfigs
    m_PlantDists.push_back(ptdc);
}

void ModelParams::addPollinatorConfig(PollinatorConfig& pc)
{
    // Set the constancy type according to the given specification string
    if (pc.strConstancyType == "none")
    {
        pc.constancyType = PollinatorConstancyType::NONE;
    }
    else if (pc.strConstancyType == "simple")
    {
        pc.constancyType = PollinatorConstancyType::SIMPLE;
    }
    else
    {
        pc.constancyType = PollinatorConstancyType::NONE;
        /// @todo... maybe output warning msg here and in next case?
    }


    // Set the foraging strategy according to the given specification string
    if (pc.strForagingStrategy == "random")
    {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM;
    }
    else if (pc.strForagingStrategy == "nearest-flower")
    {
        pc.foragingStrategy = PollinatorForagingStrategy::NEAREST_FLOWER;
    }
    else if (pc.strForagingStrategy == "random-flower")
    {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM_FLOWER;
    }
    else
    {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM;
        /// @todo... maybe output warning msg here and in next case?
    }

    m_PollinatorConfigs.push_back(pc);
}

PollinatorConfig* ModelParams::getPollinatorConfigPtr(const std::string& pollinatorName)
{
    for (PollinatorConfig& pc : m_PollinatorConfigs)
    {
        if (pc.species == pollinatorName)
        {
            return &pc;
        }
    }

    return nullptr;
}

void ModelParams::addPlantTypeConfig(PlantTypeConfig& pt)
{
    m_PlantTypes.push_back(pt);
    FloweringPlant::registerSpecies(pt.species);
}


const PlantTypeConfig* ModelParams::getPlantTypeConfig(std::string speciesName)
{
    for (auto& pt : m_PlantTypes)
    {
        if (pt.species == speciesName) return &pt;
    }
    return nullptr;
}


// perform any required global post-processing of parameter configuration after all
// details have been read from the configuration file at the start of a run
void ModelParams::postprocess()
{
    FloweringPlant::constructCloggingMap(m_PlantTypes);
}


// check consistency of parameters once the whole config file has been processed
void ModelParams::checkConsistency()
{
    switch (m_GenTerminationType)
    {
        case GenTerminationType::NUM_SIM_STEPS:
        {
            if (m_iGenTerminationParam < 0)
            {
                throw std::runtime_error("Must provide an int parameter for termination type num-sim-steps");
            }
            break;
        }
        case GenTerminationType::NUM_POLLINATOR_STEPS:
        {
            if (m_iGenTerminationParam < 0)
            {
                throw std::runtime_error("Must provide an int parameter for termination type num-pollinator-steps");
            }
            break;
        }
        case GenTerminationType::POLLINATED_FRACTION:
        {
            if (m_fGenTerminationParam < 0.0)
            {
                throw std::runtime_error("Must provide an int parameter for termination type pollinated-fraction");
            }
            break;
        }
    }

    ///@todo - other things to check re parameter consistency...
    // - are all pollinator and hive areas completely within the envionrment area?
}
