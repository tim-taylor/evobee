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
int    ModelParams::m_iLogInterGenUpdatePeriod = 1;
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
bool   ModelParams::m_bLogPollinatorsInterPhaseFull = false;
bool   ModelParams::m_bLogPollinatorsInterPhaseSummary = false;
bool   ModelParams::m_bLogFlowersInterPhaseFull = false;
bool   ModelParams::m_bLogFlowersInterPhaseSummary = false;
bool   ModelParams::m_bLogFlowersIntraPhaseFull = false;
bool   ModelParams::m_bLogFlowersIntraPhaseSummary = false;
bool   ModelParams::m_bLogFlowerMPsInterPhaseSummary = false;
bool   ModelParams::m_bLogFlowerInfoInterPhaseSummary = false;
bool   ModelParams::m_bUseLogThreads = false;
bool   ModelParams::m_bVerbose = true;
bool   ModelParams::m_bCommandLineQuiet = false;
bool   ModelParams::m_bPtdAutoDistribs = false;
int    ModelParams::m_iPtdAutoDistribNumRows = 1;
int    ModelParams::m_iPtdAutoDistribNumCols = 1;
bool   ModelParams::m_bPtdAutoDistribEqualNums = false;
float  ModelParams::m_fPtdAutoDistribDensity = 0.5;
float  ModelParams::m_fPtdAutoDistribAreaMargin = 0.0;
bool   ModelParams::m_bPtdAutoDistribRegular = true;
bool   ModelParams::m_bPtdAutoDistribSeedOutflowAllowed = false;
unsigned int ModelParams::m_sNextFreePtdcId = 1;
std::string ModelParams::m_strLogDir {"output"};
std::string ModelParams::m_strLogFinalDir {""};
std::string ModelParams::m_strLogRunName {"run"};
std::string ModelParams::m_strRngSeed {""};
std::string ModelParams::m_strNoSpecies {"NOSPECIES"};
std::vector<HiveConfig> ModelParams::m_Hives;
std::vector<PlantTypeDistributionConfig> ModelParams::m_PlantDists;
std::vector<PlantTypeConfig> ModelParams::m_PlantTypes;
std::vector<PollinatorConfig> ModelParams::m_PollinatorConfigs;
ColourSystem ModelParams::m_ColourSystem = ColourSystem::REGULAR_MARKER_POINTS;
unsigned int ModelParams::m_iTestNumber = 0;
bool   ModelParams::m_bSyntheticRegularMarkerPointsAdded = false;

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
        case 'P':
        {
            m_bLogPollinatorsInterPhaseFull = true;
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
        case 'G':
        {
            m_bLogFlowersIntraPhaseFull = true;
            break;
        }
        case 'g':
        {
            m_bLogFlowersIntraPhaseSummary = true;
            break;
        }
        case 'm':
        {
            m_bLogFlowerMPsInterPhaseSummary = true;
            break;
        }
        case 'n':
        {
            m_bLogFlowerInfoInterPhaseSummary = true;
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

void ModelParams::setLogInterGenUpdatePeriod(int p)
{
    if (p > 0)
    {
        m_iLogInterGenUpdatePeriod = p;
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
    else if ((typestr == "pollinated-fraction-all") || (typestr == "pollinated-fraction"))
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

void ModelParams::setTestNumber(unsigned int num)
{
    m_iTestNumber = num;
}

void ModelParams::addHiveConfig(HiveConfig& hc)
{
    m_Hives.push_back(hc);
}

void ModelParams::addPlantTypeDistributionConfig(PlantTypeDistributionConfig& ptdc)
{
    if (ptdc.species == "Any") ptdc.species = "any";

    // first do some sanity checking of values
    if (ptdc.species == "any" && ptdc.refuge)
    {
        std::stringstream msg;
        msg << "Not allowed to specify a PlantTypeDistribution as a refuge where species=='any'!";
        throw std::runtime_error(msg.str());
    }

    // if this is a no go area, hardcode some associated values, ignoring anything that was in config file
    if (ptdc.species == "nogo")
    {
        ptdc.density = 0.0f;
        ptdc.refuge = true;
        ptdc.refugeAlienInflowProb = 0.0f;
        ptdc.seedOutflowAllowed = false;
    }

    // assign a unique ID to this PlantTypeDistributionConfig
    ptdc.id = m_sNextFreePtdcId++;

    // and add it to our list of all PlantTypeDistributionConfigs
    m_PlantDists.push_back(ptdc);
}

void ModelParams::addPollinatorConfig(PollinatorConfig& pc)
{
    // Set the constancy type according to the given specification string
    if (pc.strConstancyType == "none") {
        pc.constancyType = PollinatorConstancyType::NONE;
    } else if (pc.strConstancyType == "simple") {
        pc.constancyType = PollinatorConstancyType::SIMPLE;
    } else if (pc.strConstancyType == "visual") {
        pc.constancyType = PollinatorConstancyType::VISUAL;
    } else {
        pc.constancyType = PollinatorConstancyType::NONE;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator constancy type (" <<
                pc.strConstancyType << "). Using type=none." << std::endl;
        }
    }

    // Set the foraging strategy according to the given specification string
    if (pc.strForagingStrategy == "random") {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM;
    } else if (pc.strForagingStrategy == "nearest-flower") {
        pc.foragingStrategy = PollinatorForagingStrategy::NEAREST_FLOWER;
    } else if (pc.strForagingStrategy == "random-flower") {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM_FLOWER;
    } else if (pc.strForagingStrategy == "random-global") {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM_GLOBAL;
    } else {
        pc.foragingStrategy = PollinatorForagingStrategy::RANDOM;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator foraging stategy (" <<
                pc.strForagingStrategy << "). Using random." << std::endl;
        }
    }

    // Set the learning strategy according to the given specification string
    if (pc.strLearningStrategy == "stay") {
        pc.learningStrategy = PollinatorLearningStrategy::STAY;
    } else if (pc.strLearningStrategy == "stay-rnd") {
        pc.learningStrategy = PollinatorLearningStrategy::STAY_RND;
    } else if (pc.strLearningStrategy == "stay-innate") {
        pc.learningStrategy = PollinatorLearningStrategy::STAY_INNATE;
    } else if (pc.strLearningStrategy == "deliberative-decisive") {
        pc.learningStrategy = PollinatorLearningStrategy::DELIBERATIVE_DECISIVE;
    } else if (pc.strLearningStrategy == "fickle-circumspect") {
        pc.learningStrategy = PollinatorLearningStrategy::FICKLE_CIRCUMSPECT;
    } else if (pc.strLearningStrategy == "none") {
        pc.learningStrategy = PollinatorLearningStrategy::NONE;
    } else {
        pc.learningStrategy = PollinatorLearningStrategy::STAY;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator learning stategy (" <<
                pc.strLearningStrategy << "). Using stay." << std::endl;
        }
    }

    // Set the step type according to the given specification string
    if (pc.strStepType == "constant") {
        pc.stepType = PollinatorStepType::CONSTANT;
    } else if (pc.strStepType == "levy") {
        pc.stepType = PollinatorStepType::LEVY;
    } else {
        pc.stepType = PollinatorStepType::CONSTANT;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator step type (" <<
                pc.strStepType << "). Using constant." << std::endl;
        }
    }

    // Set the innate preference type according to the given specification string
    if (pc.strInnatePrefType == "giurfa") {
        pc.innatePrefType = PollinatorInnatePrefType::GIURFA;
    } else if (pc.strInnatePrefType == "flat") {
        pc.innatePrefType = PollinatorInnatePrefType::FLAT;
    } else if (pc.strInnatePrefType == "hoverfly") {
        pc.innatePrefType = PollinatorInnatePrefType::HOVERFLY;
    } else if (pc.strInnatePrefType == "preset") {
        pc.innatePrefType = PollinatorInnatePrefType::PRESET;
    } else {
        pc.innatePrefType = PollinatorInnatePrefType::GIURFA;
        if (verbose()) {
            std::cout << "Warning: unrecognised pollinator innate preference type (" <<
                pc.strInnatePrefType << "). Using giurfa." << std::endl;
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
    // first do some sanity checking of values
    if (pt.flowerMPInitMin > pt.flowerMPInitMax)
    {
        std::stringstream msg;
        msg << "PlantTypeConfig error for type " << pt.species
            << ": flowerMPInitMin (" << pt.flowerMPInitMin
            << ") cannot be larger than flowerMPInitMax (" << pt.flowerMPInitMax << ")!";
        throw std::runtime_error(msg.str());
    }

    if (pt.diffMPIsDiffSpecies && (pt.flowerMPInitMin != pt.flowerMPInitMax))
    {
        // deal with case where we need to create a separate species for each
        // different marker point that might be generated
        for (MarkerPoint mp = pt.flowerMPInitMin; mp <= pt.flowerMPInitMax; mp+=10)
        {
            PlantTypeConfig newpt{pt};
            std::stringstream species;
            species << "PlantSpecies" << mp;
            newpt.species = species.str();
            newpt.flowerMPInitMin = mp;
            newpt.flowerMPInitMax = mp;
            newpt.diffMPIsDiffSpecies = false;
            m_PlantTypes.push_back(newpt);
            FloweringPlant::registerSpecies(newpt.species);
            m_bSyntheticRegularMarkerPointsAdded = true; // we'll check whether this is okay in ModelParams::checkConsistency
        }
    }
    else
    {
        // this is the straightforward default case where we just need to record a
        // single new plant type
        m_PlantTypes.push_back(pt);
        FloweringPlant::registerSpecies(pt.species);
    }
}

/**
 * Returns a pointer to the PlantTypeConfig structure associates with the given
 * plant species name.
 *
 * @note If no match is found, nullptr is returned!
 */
const PlantTypeConfig* ModelParams::getPlantTypeConfig(std::string speciesName)
{
    if (speciesName == "any")
    {
        if (m_PlantTypes.empty())
        {
            return nullptr;
        }
        else
        {
            std::uniform_int_distribution<int> dist(0, m_PlantTypes.size()-1);
            int idx = dist(EvoBeeModel::m_sRngEngine);
            return &(m_PlantTypes.at(idx));
        }
    }
    else
    {
        for (auto& pt : m_PlantTypes)
        {
            if (pt.species == speciesName) return &pt;
        }
        return nullptr;
    }
}

/**
 * Returns a pointer to the PlantTypeConfig structure identified by the given speciesNum,
 * which is simply the index number of the desired PlantTypeConfig in the m_PlantTypes
 * vector.  If the given speciesNum is greater than the number of plant types held in the
 * vector, an exception is thrown.  To guard against this, call getNumPlantTypes() to check
 * how many there are before calling this method.
 */
const PlantTypeConfig* ModelParams::getPlantTypeConfig(unsigned int speciesNum)
{
    if (speciesNum >= m_PlantTypes.size())
    {
        std::stringstream msg;
        msg << "Unknown plant species number " << speciesNum << " encountered in ModelParams::getPlantTypeConfig()";
        throw std::runtime_error(msg.str());
    }
    else {
        return &(m_PlantTypes.at(speciesNum));
    }
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


void ModelParams::setPtdAutoDistribEqualNums(bool equalnums)
{
    m_bPtdAutoDistribEqualNums = equalnums;
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


void ModelParams::setPtdAutoDistribRegular(bool regular)
{
    m_bPtdAutoDistribRegular = regular;
}


void ModelParams::setPtdAutoDistribSeedOutflowAllowed(bool allowed)
{
    m_bPtdAutoDistribSeedOutflowAllowed = allowed;
}


void ModelParams::setColourSystem(const std::string& cs)
{
    if (cs == "regular-marker-points") {
        m_ColourSystem = ColourSystem::REGULAR_MARKER_POINTS;
    }
    else if (cs == "arbitrary-dominant-wavelengths") {
        m_ColourSystem = ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS;
    }
    else {
        m_ColourSystem = ColourSystem::REGULAR_MARKER_POINTS;
        if (verbose()) {
            std::cout << "Warning: unrecognised colour system type (" << cs << "). Assuming regular-marker-points." << std::endl;
        }
    }
}


// If the config parameter auto-distribs is set to true, we now have to automatically generate
// the required set of PlantTypeDistributionConfig objects here based upon the specification
// provided by the related config parameters.
void ModelParams::autoGeneratePtds()
{
    assert(m_bPtdAutoDistribs);

    // First we initialise an 2D array (actually stored in a 1D vector) which contains
    // info about which species will go into which patch. This initialisation is done by
    // the helper method initialiseAutoGenPtdSpeciesPatchMap(), which takes into account
    // whether the param m_bPtdAutoDistribRegular is true or false, to determine
    // whether distribution is regular (checkerboard) or stochastic. Having created the
    // map, within the nested loop below we can just call another helper method
    // getAutoGenPtdSpeciesForPatch() to extract the corresponding species from the
    // array for any given x and y patch coordinate.
    std::vector<const std::string*> speciesPatchMap; // for each patch, record name of inhabitant species
    initialiseAutoGenPtdSpeciesPatchMap(speciesPatchMap);

    int areaWidth = m_iEnvSizeX / m_iPtdAutoDistribNumCols;
    int areaHeight = m_iEnvSizeY / m_iPtdAutoDistribNumRows;
    int margin = (int)(0.5 * (float)std::min(areaWidth, areaHeight) * m_fPtdAutoDistribAreaMargin);

    int curX = 0;
    int curY = 0;

    for (int x = 0; x < m_iPtdAutoDistribNumCols; x++)
    {
        for (int y = 0; y < m_iPtdAutoDistribNumRows; y++)
        {
            PlantTypeDistributionConfig ptdc;
            ptdc.species = getAutoGenPtdSpeciesForPatch(x, y, speciesPatchMap);

            ptdc.areaTopLeft.x = curX + margin;
            ptdc.areaTopLeft.y = curY + margin;
            ptdc.areaBottomRight.x = curX + areaWidth - 1 - margin;
            ptdc.areaBottomRight.y = curY + areaHeight - 1 - margin;

            if (ptdc.species == ModelParams::m_strNoSpecies)
            {
                // this is an area that will initially be empty (because m_bPtdAutoDistribEqualNums==true means
                // that we may have a few empty areas). So in this case, we give the area an initial density of
                // zero, and arbitrarily assign it to the first plant species in the map to keep the rest of
                // the code happy
                ptdc.density = 0.0;
                ptdc.species = FloweringPlant::getSpeciesMap().begin()->second;
            }
            else
            {
                ptdc.density = m_fPtdAutoDistribDensity;
            }

            ptdc.refuge = false;
            ptdc.refugeAlienInflowProb = 0.0;   // not required if refuge = false
            ptdc.seedOutflowAllowed = m_bPtdAutoDistribSeedOutflowAllowed;
            ptdc.seedOutflowRestricted = false; // not required if seedOutflowAllowed = false
            ptdc.seedOutflowProb = 1.0;         // not required if seedOutflowRestricted = false
            ptdc.reproLocalDensityConstrained = true;
            ptdc.reproLocalDensityMax = m_fPtdAutoDistribDensity;

            addPlantTypeDistributionConfig(ptdc);

            curY += areaHeight;
        }

        curX += areaWidth;
        curY = 0;
    }
}


// private helper method for ModelParams::autoGeneratePtds()
void ModelParams::initialiseAutoGenPtdSpeciesPatchMap(std::vector<const std::string*>& speciesPatchMap)
{
    assert(speciesPatchMap.empty());

    int numPatches = m_iPtdAutoDistribNumCols * m_iPtdAutoDistribNumRows;

    speciesPatchMap.reserve(numPatches);

    const std::map<unsigned int, std::string>& flowerSpeciesMap = FloweringPlant::getSpeciesMap();

    int numSpecies = flowerSpeciesMap.size();
    int numPatchesToFill = m_bPtdAutoDistribEqualNums ? numPatches - (numPatches % numSpecies) : numPatches;
    int numPatchesFilled = 0;

    if (m_bPtdAutoDistribRegular)
    {
        // allocate species to patches in a regular checkerboard pattern

        // NB This algorithm does not work well if numSpecies > m_iPtdAutoDistribNumRows/Cols!!
        assert(numSpecies <= m_iPtdAutoDistribNumRows);
        assert(numSpecies <= m_iPtdAutoDistribNumCols);

        auto nextColFirstSpeciesItr = flowerSpeciesMap.begin();
        for (int x = 0; x < m_iPtdAutoDistribNumCols; x++)
        {
            auto nextSpeciesItr = nextColFirstSpeciesItr;
            for (int y = 0; y < m_iPtdAutoDistribNumRows; y++)
            {
                if (numPatchesFilled < numPatchesToFill) {
                    speciesPatchMap.push_back(&(nextSpeciesItr->second));
                    numPatchesFilled++;
                    nextSpeciesItr++;
                    if (nextSpeciesItr == flowerSpeciesMap.end())
                    {
                        nextSpeciesItr = flowerSpeciesMap.begin();
                    }
                }
                else {
                    speciesPatchMap.push_back(&ModelParams::m_strNoSpecies);
                }
            }
            nextColFirstSpeciesItr++;
            if (nextColFirstSpeciesItr == flowerSpeciesMap.end())
            {
                nextColFirstSpeciesItr = flowerSpeciesMap.begin();
            }
        }
    }
    else {
        // species are to be distributed stochastically across the environment

        // first allocate species to patches in a regular checkerboard pattern
        auto nextSpeciesItr = flowerSpeciesMap.begin();
        for (int x = 0; x < m_iPtdAutoDistribNumCols; x++)
        {
            for (int y = 0; y < m_iPtdAutoDistribNumRows; y++)
            {
                if (numPatchesFilled < numPatchesToFill) {
                    speciesPatchMap.push_back(&(nextSpeciesItr->second));
                    numPatchesFilled++;
                    nextSpeciesItr++;
                    if (nextSpeciesItr == flowerSpeciesMap.end()) {
                        nextSpeciesItr = flowerSpeciesMap.begin();
                    }
                }
                else {
                    speciesPatchMap.push_back(&ModelParams::m_strNoSpecies);
                }
            }
        }

        // now just jumble everything up!
        std::shuffle(speciesPatchMap.begin(), speciesPatchMap.end(), EvoBeeModel::m_sRngEngine);
    }
}


// private helper method for ModelParams::autoGeneratePtds()
const std::string ModelParams::getAutoGenPtdSpeciesForPatch(int x, int y, std::vector<const std::string*>& speciesPatchMap)
{
    return *(speciesPatchMap.at(x*m_iPtdAutoDistribNumRows + y));
}


// perform any required global post-processing of parameter configuration after all
// details have been read from the configuration file at the start of a run
void ModelParams::postprocess()
{
    // if the auto generation tool has been requested for generated Plant Type Distributions,
    // run it now
    if (m_bPtdAutoDistribs)
    {
        autoGeneratePtds();
    }

    // for each plant species, create a list of which other species it clogs, for
    // efficiency purposes
    FloweringPlant::constructCloggingMap(m_PlantTypes);

    if (m_ColourSystem == ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS)
    {
        pairPlantTypeConfigsToVisData();
    }
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

    switch (m_ColourSystem) {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
            for (auto &pc : m_PollinatorConfigs) {
                if ((!pc.visDataEquallySpaced) || (pc.visDataMPStep < 1)) {
                    throw std::runtime_error("Error: with colour-system set to 'regular-marker-points' visual data must be defined at equally-spaced marker points");
                }
            }
            break;
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
        {
            if (m_bSyntheticRegularMarkerPointsAdded) {
                throw std::runtime_error("Error: with colour-system set to 'arbitrary-dominant-wavelengths' all PlantTypeConfigs must have flowerMPInitMin=flowerMPInitMax");
            }

            for (auto& pc : m_PollinatorConfigs) {
                pc.visDataMPStep = 1; // step has no meaning for ARBITRARY_DOMINANT_WAVELENGTHS, so we just ensure it is set to 1 here rather than some random other number
                if (pc.learningStrategy != PollinatorLearningStrategy::STAY_INNATE) {
                    // At present the code has only been designed to cope with STAY_INNATE for ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS, so
                    // throw an error if this is not the case
                    throw std::runtime_error("Error: with colour-system set to 'arbitrary-dominant-wavelengths' all pollinator's learning strategies should be set to 'stay-innate'");
                }
            }

            auto& visData = getVisData();

            // Check that each entry in PollinatorConfig::visData matches a type of plant in the PlantTypeConfigs, and vice versa
            bool allOK = true;
            if (m_PlantTypes.size() != visData.size()) {
                allOK = false;
            }
            else {
                // go through plant types one by one checking that it has been successfully matched to an visdata entry
                // (this should already be guaranteed by the action of ModelParams::pairPlantTypeConfigsToVisData())
                for (auto& ptc : m_PlantTypes) {
                    if (ptc.flowerVisDataPtr == nullptr) {
                        allOK = false;
                        break;
                    }
                }
            }
            if (!allOK) {
                throw std::runtime_error("Error: each dominant wavelength associated with a PlantTypeConfig must match an entry in the vis-data array and vice versa");
            }
            break;
        }
        default:
        {
            throw std::runtime_error("Error: encountered unknown ColourSystem in ModelParams::checkConsistency(). Aborting!");
        }
    }

    ///@todo - other things to check re parameter consistency...
    // - are all pollinator and hive areas completely within the environment area?
}


// for each PlantTypeConfig, check that its flowerVisDataID matches an ID in VisData, and initialise a pointer in the
// PlantTypeConfig to point to the corresponding VisData entry.  If no match is found for a PlantTypeConfig, throw an
// exception.
void ModelParams::pairPlantTypeConfigsToVisData()
{
    // this method is only designed to be used with ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS
    assert(m_ColourSystem == ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS);

    auto& visData = getVisData();

    for (auto& ptc : m_PlantTypes)
    {
        // first check our assumption that flower marker points have not been set explicitly in this case
        assert(ptc.flowerMPInitMin == 0);
        assert(ptc.flowerMPInitMax == 0);

        // and check that the vis-data-id has been set
        assert(ptc.flowerVisDataID >= 0);

        int visDataID = ptc.flowerVisDataID;

        auto it = std::find_if(
            visData.begin(),
            visData.end(),
            [visDataID](const VisualStimulusInfo& info) {return (visDataID == info.id);}
        );

        if (it == visData.end()) {
            std::stringstream msg;
            msg << "In ModelParams::pairPlantTypeConfigsToVisData(): unable to find matching entry in vis-data\n"
                << " for PlantTypeConfig with flower-vis-data-id = " << visDataID << "\n";
            throw std::runtime_error(msg.str());
        }

        ptc.flowerVisDataPtr = &(*it);
    }
}


const std::vector<VisualStimulusInfo>& ModelParams::getVisData()
{
    // this method is only designed to be used with ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS
    assert(m_ColourSystem == ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS);

    // At present the code assumes that there is only one single pollinator type when using
    // ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS
    if (m_PollinatorConfigs.size() != 1) {
        throw std::runtime_error("Error: ModelParams::getVisData() only works with a single pollinator type at present");
    }

    return m_PollinatorConfigs.at(0).visData;
}
