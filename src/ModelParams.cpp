/**
 * @file
 *
 * Implementation of the ModelParams class
 */

#include <exception>
#include "tools.h"
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
bool   ModelParams::m_bLogPollinatorsIntraPhaseFull = false;
bool   ModelParams::m_bLogPollinatorsInterPhaseSummary = false;
bool   ModelParams::m_bLogFlowersInterPhaseFull = false;
bool   ModelParams::m_bLogFlowersInterPhaseSummary = false;
bool   ModelParams::m_bLogFlowersIntraPhaseSummary = false;
bool   ModelParams::m_bUseLogThreads = false;
bool   ModelParams::m_bVerbose = true;
bool   ModelParams::m_bPtdAutoDistribs = false;
int    ModelParams::m_iPtdAutoDistribNumRows = 1;
int    ModelParams::m_iPtdAutoDistribNumCols = 1;
float  ModelParams::m_fPtdAutoDistribDensity = 0.5;
float  ModelParams::m_fPtdAutoDistribAreaMargin = 0.0;
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
        if (verbose()) {
            std::cout << "Warning: max screen frac W set too low (" << fw << "). Using 0.1 instead." << std::endl;
        }
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
        if (verbose()) {
            std::cout << "Warning: max screen frac H set too low (" << fh << "). Using 0.1 instead." << std::endl;
        }
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
        case 'Q':
        {
            m_bLogPollinatorsIntraPhaseFull = true;
            break;
        }
        case 'p':
        {
            m_bLogPollinatorsInterPhaseSummary = true;
            break;
        }
        case 'F':
        {
            m_bLogFlowersInterPhaseFull = true;
            break;
        }
        case 'f':
        {
            m_bLogFlowersInterPhaseSummary = true;
            break;
        }
        case 'g':
        {
            m_bLogFlowersIntraPhaseSummary = true;
            break;
        }
        default:
        {
            std::cerr << "Warning: Ignoring unknown log flag '" << flag << "'" << std::endl;
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
    else if (typestr == "pollinated-fraction-all")
    {
        m_GenTerminationType = GenTerminationType::POLLINATED_FRACTION_ALL;
    }
    else if (typestr == "pollinated-fraction-species1")
    {
        m_GenTerminationType = GenTerminationType::POLLINATED_FRACTION_SPECIES1;
    }
    else
    {
        m_GenTerminationType = GenTerminationType::NUM_SIM_STEPS;
        if (verbose()) {
            std::cout << "Warning: unrecognised generation termination type (" <<
                typestr << "). Using num-sim-steps." << std::endl;
        }
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
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator constancy type (" <<
                pc.strConstancyType << "). Using type=none." << std::endl;
        }
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
    else if (pc.strForagingStrategy == "random-global")
    {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM_GLOBAL;
    }
    else
    {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator foraging stategy (" <<
                pc.strForagingStrategy << "). Using random." << std::endl;
        }
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


void ModelParams::setPtdAutoDistribs(bool bAutoDistribs)
{
    m_bPtdAutoDistribs = bAutoDistribs;
}


void ModelParams::setPtdAutoDistribNumRows(int rows)
{
    if (rows < 1)
    {
        throw std::runtime_error("Invalid Plant Type Distribution auto distribution num rows (must be >= 1)");
    }

    m_iPtdAutoDistribNumRows = rows;
}


void ModelParams::setPtdAutoDistribNumCols(int cols)
{
    if (cols < 1)
    {
        throw std::runtime_error("Invalid Plant Type Distribution auto distribution num columns (must be >= 1)");
    }

    m_iPtdAutoDistribNumCols = cols;
}


void ModelParams::setPtdAutoDistribDensity(float density)
{
    if (density < EvoBee::FLOAT_COMPARISON_EPSILON)
    {
        throw std::runtime_error("Plant Type Distribution auto distribution density must be positive");
    }

    m_fPtdAutoDistribDensity = density;
}


void ModelParams::setPtdAutoDistribAreaMargin(float margin)
{
    if ((margin < -EvoBee::FLOAT_COMPARISON_EPSILON) || (margin > 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON))
    {
        throw std::runtime_error("Invalid Plant Type Distribution auto distribution area margin (must be in range (0,1)");
    }

    m_fPtdAutoDistribAreaMargin = margin;
}


void ModelParams::autoGeneratePtds()
{
    assert(m_bPtdAutoDistribs);

    int areaWidth = m_iEnvSizeX / m_iPtdAutoDistribNumCols;
    int areaHeight = m_iEnvSizeY / m_iPtdAutoDistribNumRows;
    int margin = (int)(0.5 * (float)std::min(areaWidth, areaHeight) * m_fPtdAutoDistribAreaMargin);

    int curX = 0;
    int curY = 0;

    const std::map<unsigned int, std::string>& flowerSpeciesMap = FloweringPlant::getSpeciesMap();
    auto nextColFirstSpeciesItr = flowerSpeciesMap.begin();

    for (int x = 0; x < m_iPtdAutoDistribNumCols; x++)
    {
        auto nextSpeciesItr = nextColFirstSpeciesItr;

        for (int y = 0; y < m_iPtdAutoDistribNumRows; y++)
        {
            PlantTypeDistributionConfig ptdc;
            ptdc.species = nextSpeciesItr->second;
            ptdc.areaTopLeft.x = curX + margin;
            ptdc.areaTopLeft.y = curY + margin;
            ptdc.areaBottomRight.x = curX + areaWidth - 1 - margin;
            ptdc.areaBottomRight.y = curY + areaHeight - 1 - margin;
            ptdc.density = m_fPtdAutoDistribDensity;

            addPlantTypeDistributionConfig(ptdc);

            nextSpeciesItr++;
            if (nextSpeciesItr == flowerSpeciesMap.end())
            {
                nextSpeciesItr = flowerSpeciesMap.begin();
            }

            curY += areaHeight;
        }

        nextColFirstSpeciesItr++;
        if (nextColFirstSpeciesItr == flowerSpeciesMap.end())
        {
            nextColFirstSpeciesItr = flowerSpeciesMap.begin();
        }

        curX += areaWidth;
        curY = 0;
    }
}


// perform any required global post-processing of parameter configuration after all
// details have been read from the configuration file at the start of a run
void ModelParams::postprocess()
{
    // if the auto generation tools has been requested for generated Plant Type Distributions,
    // envoke it now
    if (m_bPtdAutoDistribs)
    {
        autoGeneratePtds();
    }

    // for each plant species, create a list of which other species it clogs, for
    // efficiency purposes
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
        case GenTerminationType::POLLINATED_FRACTION_ALL:
        {
            if (m_fGenTerminationParam <= EvoBee::FLOAT_COMPARISON_EPSILON || m_fGenTerminationParam > 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON)
            {
                throw std::runtime_error("Must provide a float parameter in range [0.0,1.0) for termination type pollinated-fraction-all");
            }
            break;
        }
        case GenTerminationType::POLLINATED_FRACTION_SPECIES1:
        {
            if (m_fGenTerminationParam <= EvoBee::FLOAT_COMPARISON_EPSILON || m_fGenTerminationParam > 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON)
            {
                throw std::runtime_error("Must provide an float parameter in range [0.0,1.0) for termination type pollinated-fraction-species1");
            }
            break;
        }
        default:
        {
            throw std::runtime_error("Error: encountered unknown GenTerminationType in ModelParams::checkConsistency(). Aborting!");
        }
    }

    ///@todo - other things to check re parameter consistency...
    // - are all pollinator and hive areas completely within the envionrment area?
}
