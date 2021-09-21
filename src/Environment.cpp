/**
 * @file
 *
 * Implementation of the Environment class and LocalDensityConstraint class
 */

#include <cassert>
#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include <cmath>
#include "tools.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "Patch.h"
#include "Colour.h"
#include "AbstractHive.h"
#include "Hive.h"
#include "HoneyBee.h"
#include "Position.h"
#include "FloweringPlant.h"
#include "Environment.h"


bool LocalDensityConstraint::posInArea(const iPos& pos) const
{
    return Environment::inArea(pos, ptdcfg.areaTopLeft, ptdcfg.areaBottomRight);
}


Environment::Environment(EvoBeeModel* pModel) :
    m_bFlowerPtrVectorInitialised(false),
    m_pModel(pModel)
{
    assert(ModelParams::initialised());

    m_iSizeX = ModelParams::getEnvSizeX();
    m_iSizeY = ModelParams::getEnvSizeY();
    m_fSizeX = (float)m_iSizeX;
    m_fSizeY = (float)m_iSizeY;

    // Initialise Patches
    m_iNumPatches = ModelParams::getNumPatches();
    m_Patches.reserve(m_iNumPatches);
    for (int i = 0; i < m_iNumPatches; ++i)
    {
        m_Patches.emplace_back(
            this, i,
            ModelParams::getEnvBackgroundReflectanceMP(),
            ModelParams::getEnvDefaultAmbientTemp()
        );
    }

    // Initialise Plants
    initialisePlants();

    // Initialise internal book-keeping for local density limits during plant reproduction
    initialiseLocalDensityCounts();

    // Initialise Hives
    //
    // N.B. this has to happen *after* initialisePlants() is called, because that deals with
    //  any no-go areas in the environemnt, which we might have to take account of when
    //  deciding the initial placement of the pollinators from the hive.
    //
    const std::vector<HiveConfig> & hconfigs = ModelParams::getHiveConfigs();
    for (const HiveConfig& hconfig : hconfigs)
    {
        // Use the AbstractHive::makeHive factory method to make a Hive of the right type
        m_Hives.push_back( AbstractHive::makeHive(this, hconfig) );
    }

    if (ModelParams::verbose())
    {
        std::cout << "Initialised Environment with " << m_Patches.size() << " patches" << std::endl;
    }
}


iPos Environment::getPatchCoordsFromIdx(int idx)
{
    assert(idx >= 0);
    assert(idx < m_iNumPatches);
    return iPos(idx % m_iSizeX, idx / m_iSizeX);
}


Patch& Environment::getPatch(int x, int y)
{
    int idx = x + (m_iSizeX * y);

    if (!inEnvironment(x,y))
    {
        assert(inEnvironment(x,y));
    }

    return m_Patches[idx];
}


/**
 * This is a private helper method called by the constructor.
 *
 * It creates the required number and distribution of plants as specified in the config file.
 *
 * This method is somewhat convoluted for efficiency purposes. Having figured out how
 * many plants in total should be created, it then randomly assigns positions for each
 * plant. These positions are stored in the patchInfo[][] array, where each element in
 * the array is a vector containing the positions of all new plants that fall within
 * that patch. We then loop through the patchInfo array, and for each vector contained therein,
 * we create all of the plants for the associated patch.
 */
void Environment::initialisePlants()
{
    const std::vector<PlantTypeDistributionConfig>& pdconfigs =
        ModelParams::getPlantTypeDistributionConfigs();

    for (const PlantTypeDistributionConfig& pdcfg : pdconfigs)
    {
        // calculate some basic values associated with the requested distribution
        int w = std::max(1, 1 + pdcfg.areaBottomRight.x - pdcfg.areaTopLeft.x);
        int h = std::max(1, 1 + pdcfg.areaBottomRight.y - pdcfg.areaTopLeft.y);
        int area = w * h;
        int numPlants = (pdcfg.species == "nogo") ? 0 : (int)((float)area * pdcfg.density);
        std::uniform_real_distribution<float> distW(0.0, w - EvoBee::SMALL_FLOAT_NUMBER);
        std::uniform_real_distribution<float> distH(0.0, h - EvoBee::SMALL_FLOAT_NUMBER);

        // create a 2D array that will record, for each patch in the distribution area,
        // the coordinates of each plant to be added to that patch
        std::vector<fPos> patchInfo[w][h];

        for (int i = 0; i < numPlants; ++i)
        {
            fPos fpos{ pdcfg.areaTopLeft.x + distW(EvoBeeModel::m_sRngEngine),
                       pdcfg.areaTopLeft.y + distH(EvoBeeModel::m_sRngEngine) };
            int iLocalX = (int)fpos.x - pdcfg.areaTopLeft.x;
            int iLocalY = (int)fpos.y - pdcfg.areaTopLeft.y;
            patchInfo[iLocalX][iLocalY].push_back(fpos);
        }

        const PlantTypeConfig* pPTC = nullptr;
        std::vector<unsigned int> anyPlantDistribInfo;

        if ((pdcfg.species != "any") && (pdcfg.species != "nogo"))
        {
            // find the corresponding PlantTypeConfig (unless pdcfg.species is "any" or "nogo")
            pPTC = ModelParams::getPlantTypeConfig(pdcfg.species);
            if (pPTC == nullptr)
            {
                throw std::runtime_error("Unknown plant species '" + pdcfg.species +
                    "' specified in config file");
            }
        }
        else if (pdcfg.species == "any")
        {
            // if pdcfg.species is "any", prepare a data structure to ensure we create an equal
            // number of each plant species
            int numSpecies = ModelParams::getNumPlantTypes();
            for (int i=0; i<numPlants; i++)
            {
                anyPlantDistribInfo.push_back(i%numSpecies);
            }
            std::shuffle(anyPlantDistribInfo.begin(), anyPlantDistribInfo.end(), EvoBeeModel::m_sRngEngine);
        }
        auto anyPlantDistribInfoItr = anyPlantDistribInfo.begin();


        // now go through the patchInfo array, and for each patch, go through all plants
        // to be added to that patch
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; ++y)
            {
                Patch& patch = getPatch(x + pdcfg.areaTopLeft.x, y + pdcfg.areaTopLeft.y);
                std::vector<fPos>& posvec = patchInfo[x][y];
                for (fPos& pos : posvec)
                {
                    if (pdcfg.species == "any")
                    {
                        assert(anyPlantDistribInfoItr != anyPlantDistribInfo.end());
                        pPTC = ModelParams::getPlantTypeConfig(*anyPlantDistribInfoItr);
                        anyPlantDistribInfoItr++;

                        /*
                        pPTC = ModelParams::getPlantTypeConfig(pdcfg.species);
                        if (pPTC == nullptr)
                        {
                            throw std::runtime_error("Unknown plant species '" + pdcfg.species +
                                "' specified in config file");
                        }
                        */
                    }

                    patch.addPlant(*pPTC, pos);
                }

                if (pdcfg.refuge || pdcfg.seedOutflowRestricted || (!pdcfg.seedOutflowAllowed))
                {
                    patch.setReproConstraints(pdcfg);
                }
            }
        }
    }
}


fPos Environment::getRandomPositionF() const
{
    static std::uniform_real_distribution<float> distX(0.0, m_fSizeX);
    static std::uniform_real_distribution<float> distY(0.0, m_fSizeY);
    return fPos(distX(EvoBeeModel::m_sRngEngine), distY(EvoBeeModel::m_sRngEngine));
}


fPos Environment::getRandomPositionF(const iPos& topleft, const iPos& bottomright) const
{
    std::uniform_real_distribution<float> distX((float)topleft.x, (float)(bottomright.x+1));
    std::uniform_real_distribution<float> distY((float)topleft.y, (float)(bottomright.y+1));
    return fPos(distX(EvoBeeModel::m_sRngEngine), distY(EvoBeeModel::m_sRngEngine));
}


iPos Environment::getPatchCoordFromFloatPos(const fPos& fpos)
{
    return iPos { (int)std::floor(fpos.x), (int)std::floor(fpos.y) };
}


bool Environment::inEnvironment(int x, int y) const
{
    return (
        x >=0 &&
        x < m_iSizeX &&
        y >= 0 &&
        y < m_iSizeY
    );
}


bool Environment::inArea(const iPos& pos, const iPos& areaTopLeft, const iPos& areaBottomRight)
{
    return (
        (pos.x >= areaTopLeft.x) &&
        (pos.x <= areaBottomRight.x) &&
        (pos.y >= areaTopLeft.y) &&
        (pos.y <= areaBottomRight.y)
    );
}


// Search for plants in the local patch and its 8 closest neighbours
// (Moore neighbourhood), and return a pointer to the closest plant found,
// or nullptr if none found.
//
// The second parameter (default value = 1.0) specifies a maximum search radius.
// If this is given a zero or negative value, then it is ignored, and all flowers
// within the Moore neighbourhood are considered regardless of distance (but
// note that this means that the search radius is effectively asymmetric in
// different directions because the patches are squares). If the radius is set
// to a positive value, then only flowers within that distance of fpos will be
// considered. Therefore, if fRadius is positive, it only really makes sense for
// it to have a value of <= 1.0, as above 1.0 the search might be clipped depending
// upon the location of fpos within the central patch.
//
// The third parameter determines whether a flower at the given position (fpos)
// should be considered (it is set to true by default, meaning a flower at the
// current position will not be considered).
//
FloweringPlant* Environment::findNearestFloweringPlant( const fPos& fpos,
                                                        float fRadius /*= 1.0*/,
                                                        bool excludeCurrentPos /*= true*/)
{
    assert(fRadius < 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON);

    FloweringPlant* pPlant = nullptr;
    float minDistSq = 9999999.9;

    // search for flowers within the Moore neighbourhood of the specified
    // position, and, if any found, record which is the closest
    if (inEnvironment(fpos))
    {
        iPos ipos = getPatchCoordFromFloatPos(fpos);

        for (int x = ipos.x - 1; x <= ipos.x + 1; ++x)
        {
            if (x >= 0 && x < m_iSizeX)
            {
                for (int y = ipos.y - 1; y <= ipos.y + 1; ++y)
                {
                    if (y >= 0 && y < m_iSizeY)
                    {
                        // for each patch in Moore neighbourhood...
                        Patch& patch = getPatch(x,y);
                        PlantVector& plants = patch.getFloweringPlants();
                        for (FloweringPlant& plant : plants)
                        {
                            // for each plant in patch...
                            float distSq = plant.getDistanceSq(fpos);
                            if (distSq < minDistSq)
                            {
                                if ((!excludeCurrentPos) || (distSq > EvoBee::FLOAT_COMPARISON_EPSILON))
                                {
                                    // this is the closest eligible plant we've found so far, so record it!
                                    minDistSq = distSq;
                                    pPlant = &plant;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if a maximum search radius has been specified and the closest flower is
    // beyond that distance, ignore it and return nullptr instead
    if ((fRadius > EvoBee::FLOAT_COMPARISON_EPSILON) && (minDistSq > (fRadius * fRadius)))
    {
        pPlant = nullptr;
    }

    return pPlant;
}


// Search for flowers in the local patch and its 8 closest neighbours
// (Moore neighbourhood), and return a pointer to the closest flower found
// that is not in the supplied list of excluded flowers, or nullptr if none found.
// Optionally, a Pollinator may be supplied as the final argument
// (pPollinator), which, if present, only considers flowers that the
// Pollinator can detect (as determined by calling its isDetected(reflectanceInfo)
// method).
//
// The third parameter (default value = 1.0) specifies a maximum search radius.
// If this is given a zero or negative value, then it is ignored, and all flowers
// within the Moore neighbourhood are considered regardless of distance (but
// note that this means that the search radius is effectively asymmetric in
// different directions because the patches are squares). If the radius is set
// to a positive value, then only flowers within that distance of fpos will be
// considered. Therefore, if fRadius is positive, it only really makes sense for
// it to have a value of <= 1.0, as above 1.0 the search might be clipped depending
// upon the location of fpos within the central patch.
//
// The fourth parameter determines whether a flower at the given position (fpos)
// should be considered (it is set to true by default, meaning a flower at the
// current position will not be considered).
//
Flower *Environment::findNearestUnvisitedFlower(const fPos &fpos,
                                                const std::vector<Flower*>& excludeVec,
                                                float fRadius /*= 1.0*/,
                                                bool excludeCurrentPos /*= true*/,
                                                Pollinator* pPollinator /*= nullptr*/)
{
    assert(fRadius < 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON);

    Flower* pFlower = nullptr;
    bool checkMaxRadius = (fRadius > EvoBee::FLOAT_COMPARISON_EPSILON);
    float minDistSq = 9999999.9;

    // search for flowers within the Moore neighbourhood of the specified
    // position, and, if any found, record which is the closest
    if (inEnvironment(fpos))
    {
        iPos ipos = getPatchCoordFromFloatPos(fpos);

        for (int x = ipos.x - 1; x <= ipos.x + 1; ++x)
        {
            if (x >= 0 && x < m_iSizeX)
            {
                for (int y = ipos.y - 1; y <= ipos.y + 1; ++y)
                {
                    if (y >= 0 && y < m_iSizeY)
                    {
                        // for each patch in Moore neighbourhood...
                        Patch &patch = getPatch(x, y);
                        PlantVector& plants = patch.getFloweringPlants();
                        for (FloweringPlant& plant : plants)
                        {
                            // for each plant in patch...
                            std::vector<Flower>& flowers = plant.getFlowers();
                            for (Flower& flower : flowers)
                            {
                                // for each flower on plant...
                                unsigned int flowerID = flower.getId();
                                if (std::find_if(excludeVec.begin(),
                                                 excludeVec.end(),
                                                 [flowerID](Flower* pOtherFlower){return (pOtherFlower->getId() == flowerID);})
                                    == excludeVec.end())
                                {
                                    // if flower not on exclude list...
                                    float distSq = EvoBee::distanceSq(fpos, flower.getPosition());
                                    if (distSq < minDistSq)
                                    {
                                        // this flower is closer than the closest eligible flower we've found so far...
                                        if ((!excludeCurrentPos) || (distSq > EvoBee::FLOAT_COMPARISON_EPSILON))
                                        {
                                            // it's either not at the central focus position or we don't care if it is...
                                            if (pPollinator == nullptr ||
                                                pPollinator->isDetected(flower.getReflectanceInfo())) {
                                                // if we care about whether the pollinator can detect the flower, then
                                                // yes, it can detect it...

                                                // this is the closest eligible flower we've found so far, so record it!
                                                minDistSq = distSq;
                                                pFlower = &flower;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if a maximum search radius has been specified and the closest flower is
    // beyond that distance, ignore it and return nullptr instead
    if ((checkMaxRadius) && (minDistSq > (fRadius * fRadius)))
    {
        pFlower = nullptr;
    }

    return pFlower;
}

// Search for flowers in the local patch and its 8 closest neighbours
// (Moore neighbourhood), and return a pointer to a randomly selected found flower
// that is not in the supplied list of excluded flowers, or nullptr if none found.
//
// The third parameter (default value = 1.0) specifies a maximum search radius.
// If this is given a zero or negative value, then it is ignored, and all flowers
// within the Moore neighbourhood are considered regardless of distance (but
// note that this means that the search radius is effectively asymmetric in
// different directions because the patches are squares). If the radius is set
// to a positive value, then only flowers within that distance of fpos will be
// considered. Therefore, if fRadius is positive, it only really makes sense for
// it to have a value of <= 1.0, as above 1.0 the search might be clipped depending
// upon the location of fpos within the central patch.
//
// The fourth parameter determines whether a flower at the given position (fpos)
// should be considered (it is set to true by default, meaning a flower at the
// current position will not be considered).
//
Flower *Environment::findRandomUnvisitedFlower(const fPos &fpos,
                                               const std::vector<Flower*> &excludeVec,
                                               float fRadius /*= 1.0*/,
                                               bool excludeCurrentPos /*= true*/)
{
    assert(fRadius < 1.0 + EvoBee::FLOAT_COMPARISON_EPSILON);

    Flower *pFlower = nullptr;
    bool checkMaxRadius = (fRadius > EvoBee::FLOAT_COMPARISON_EPSILON);
    std::vector<Flower*> candidates;

    // search for flowers within the Moore neighbourhood of the specified
    // position, and, if any found, record which is the closest
    if (inEnvironment(fpos))
    {
        iPos ipos = getPatchCoordFromFloatPos(fpos);

        for (int x = ipos.x - 1; x <= ipos.x + 1; ++x)
        {
            if (x >= 0 && x < m_iSizeX)
            {
                for (int y = ipos.y - 1; y <= ipos.y + 1; ++y)
                {
                    if (y >= 0 && y < m_iSizeY)
                    {
                        // for each patch in Moore neighbourhood...
                        Patch& patch = getPatch(x, y);
                        PlantVector& plants = patch.getFloweringPlants();
                        for (FloweringPlant& plant : plants)
                        {
                            // for each plant in patch...
                            std::vector<Flower>& flowers = plant.getFlowers();
                            for (Flower& flower : flowers)
                            {
                                // for each flower on plant...
                                unsigned int flowerID = flower.getId();
                                if (std::find_if(excludeVec.begin(),
                                                 excludeVec.end(),
                                                 [flowerID](Flower* pOtherFlower){return (pOtherFlower->getId() == flowerID);})
                                    == excludeVec.end())
                                {
                                    // if flower not on exclude list...
                                    float distSq = EvoBee::distanceSq(fpos, flower.getPosition());
                                    if (((!excludeCurrentPos) || (distSq > EvoBee::FLOAT_COMPARISON_EPSILON)) &&
                                        ((!checkMaxRadius) || (distSq <= (fRadius * fRadius))))
                                    {
                                        // this is an eligible flower, so record it!
                                        candidates.push_back(&flower);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if we have found some eligible flowers, then pick one at random to return
    if (!candidates.empty())
    {
        std::uniform_int_distribution<unsigned int> dist(0, candidates.size()-1);
        pFlower = candidates.at(dist(EvoBeeModel::m_sRngEngine));
    }

    return pFlower;
}


// NB for the moment this metho assumes that plants just have one flower
float Environment::getPollinatedFracAll() const
{
    unsigned int numPlants = 0;
    unsigned int numPollinated = 0;

    for (const Patch& p : m_Patches)
    {
        if (p.hasFloweringPlants())
        {
            const PlantVector& fplants = p.getFloweringPlants();
            numPlants += fplants.size();
            for (const FloweringPlant& fplant : fplants)
            {
                if (fplant.pollinated())
                {
                    ++numPollinated;
                }
            }
        }
    }

    return (numPlants == 0) ? 0.0 : ((float)numPollinated) / ((float)numPlants);
}


// NB for the moment this metho assumes that plants just have one flower
float Environment::getPollinatedFracSpecies1() const
{
    unsigned int numPlants = 0;
    unsigned int numPollinated = 0;

    for (const Patch& p : m_Patches)
    {
        if (p.hasFloweringPlants())
        {
            for (const FloweringPlant& fplant : p.getFloweringPlants())
            {
                if (fplant.getSpeciesId() == 1)
                {
                    ++numPlants;
                    if (fplant.pollinated())
                    {
                        ++numPollinated;
                    }
                }
            }
        }
    }

    return (numPlants == 0) ? 0.0 : ((float)numPollinated) / ((float)numPlants);
}


/**
 * Create a new generation of plants based upon pollinated plants from
 * the previous generation, then delete all plants from previous generation.
 *
 * 1. Construct a new generation of plants based upon those successfully
 *    pollinated in the previous generation, taking into account any refuges
 *    and/or restrictions to seed flow.
 * 2. Reset all pollinators to initial state.
 *
 * Some more details:
 *  overall goal:
 *      replace all plants (stored in individual Patches) with a
 *      new generation of plants
 *  assumptions:
 *      If PTD area is refuge or seedOutflow is restricted (outflow prob < 1.0), that
 *          area is not allowed to overlap with any other area
 *      (see implementation details below)
 *  approach:
 *      create a vector of all pollinated plants (pollinatedPlantPtrs) and shuffle it
 *          [NB each plant can be added multiple times to the vector, once for each
 *           conspecific pollen grain found on the stigma]
 *      create an empty newPlants vector to store new generation
 *      for each plant in pollinatedPlantPtrs (up to global max repro num for env)
 *          consider a nearby position in which to reproduce
 *          determine prob that it will successfully reproduce in that position, taking into acccount
 *              seed outflow prob of current patch
 *              seed inflow prob (refuge incoming) of destination
 *              any local PTDConfig density constraints (consult m_LocalDensityConstraints)
 *          if successful, create new plant and put in newPlants vector
 *      empty plant vectors for all patches - delete all plants from previous generation
 *      for each plant in newPlants, move plant to appropriate Patch vector
 *  implementation details:
 *      each Patch needs
 *          outflow prob (and allowed/restricted flags, corresponding to p>0, p<1)
 *          isRefuge flag
 *              refugeNativeSpecies id
 *              alienInflowProb
 *      We deal with constraints on overlapping PTD areas (see assumptions above) by
 *          thowing an exception if there is an attempt to set these Patch probs/flags
 *          multiple times for any single Patch (this is done in Patch::setReproConstraints)
 */
void Environment::initialiseNewGeneration()
{
    //////////////////////////////////////////////////////////////
    // Step 0: Internal book-keeping
    resetLocalDensityCounts();

    //////////////////////////////////////////////////////////////
    // Step 1: Construct a new generation of plants

    // -- Step 1a: create a vector of all pollinated plants and shuffle it
    std::vector<FloweringPlant*> pollinatedPlantPtrs;
    for (Patch& p : m_Patches)
    {
        if (p.hasFloweringPlants())
        {
            PlantVector& plants = p.getFloweringPlants();
            for (FloweringPlant& plant : plants)
            {
                if (plant.pollinated())
                {
                    // for each pollinated plant, we look at each pollen grain on
                    // the stigma of each of its flowers, and add a candidate
                    // pointer for reproduction for each conspecific grain
                    const std::vector<Flower>& flowers = plant.getFlowers();
                    for (const Flower& flower : flowers)
                    {
                        const PollenVector& stigmaPollen = flower.getStigmaPollen();
                        for (const Pollen& pollen : stigmaPollen)
                        {
                            if (pollen.speciesId == flower.getSpeciesId())
                            {
                                pollinatedPlantPtrs.push_back(&plant);
                                ///@todo TODO when we implement mutation, we will actually want
                                /// to record the plant AND a ptr to the pollinating flower
                                /// (pollen.pSource)
                            }
                        }
                    }
                }
            }
        }
    }
    std::shuffle(pollinatedPlantPtrs.begin(), pollinatedPlantPtrs.end(), EvoBeeModel::m_sRngEngine);

    // -- Step 1b: create an empty vector to store new generation
    std::vector<FloweringPlant> newPlants;

    // -- Step 1c: for each plant in vector (up to global max repro num for env)
    unsigned int globalMax = ModelParams::getReproGlobalDensityConstrained() ?
        ((unsigned int)((float)m_Patches.size() * ModelParams::getReproGlobalDensityMax())) : 0;

    for (FloweringPlant* pPlant : pollinatedPlantPtrs)
    {
        // -- Step 1c.1: consider a possible position in which to reproduce
        const Patch& curPatch = pPlant->getPatch();

        fPos fCurPos { pPlant->getPosition() };
        fPos fNewPos;

        if (pPlant->reproSeedDispersalGlobal())
        {
            if (curPatch.seedOutflowAllowed())
            {
                // seed outflow from this patch's locality is allowed, so we now need
                // to decide whether we can pick a destination position from the entire
                // environment or just from the locality defined by m_ReproRestrictionAreaTL/BR
                if ((!curPatch.seedOutflowRestricted()) ||
                    (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < curPatch.getSeedOutflowProb()))
                {
                    fNewPos = getRandomPositionF();
                }
                else
                {
                    fNewPos = getRandomPositionF(curPatch.getReproRestrictionAreaTopLeft(),
                                                 curPatch.getReproRestrictionAreaBottomRight());
                }
            }
            else
            {
                // seed outflow from this patch's locaility is not allowed, so in
                // this case "SeedDispersalGlobal" means anywhere within the locaility
                // defined by m_ReproRestrictionAreaTL/BR
                fNewPos = getRandomPositionF(curPatch.getReproRestrictionAreaTopLeft(),
                                             curPatch.getReproRestrictionAreaBottomRight());
            }
        }
        else
        {
            // pick position from within a specified radius of the current position
            std::normal_distribution<float> distanceDistrib(0.0f, pPlant->reproSeedDispersalRadiusStdDev());
            float distance = distanceDistrib(EvoBeeModel::m_sRngEngine);
            float heading  = EvoBeeModel::m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);
            fPos delta { distance*std::cos(heading), distance*std::sin(heading) };
            fNewPos = fCurPos + delta;
        }

        iPos iCurPos = getPatchCoordFromFloatPos(fCurPos);
        iPos iNewPos = getPatchCoordFromFloatPos(fNewPos);

        // -- Step 1c.2: determine prob that it will successfully reproduce in that position,
        //    taking into acccount:
        //    - seed outflow prob of current patch
        //    - seed inflow prob (refuge incoming) of destination
        //    - any local PTDConfig density constraints (stored in ModelParams)
        bool  bAnyChance  = true;
        float successProb = 1.0;

        if (iNewPos != iCurPos)
        {
            if (!inEnvironment(iNewPos))
            {
                // new plant has fallen off the edge of the world!
                bAnyChance = false;
            }
            else
            {
                // new plant is still in environment but in a different patch to old one.
                // consider chances of successfully moving into the new patch.
                // N.B. if the new patch is a no-go area, this will have been
                // explicitly set up to be a refuge with alienInflowProb = 0.0
                // (see ModelParams::addPlantTypeDistributionConfig) - therefore in this
                // case the following code will set successProb to 0.0.
                Patch& newPatch = getPatch(iNewPos);
                if (newPatch.refuge() && (newPatch.getRefugeNativeSpeciesId() != pPlant->getSpeciesId()))
                {
                    // trying to move into a refuge for a different plant species
                    // (multiply the prob of doing this with our currently calculated prob)
                    successProb *= newPatch.getRefugeAlienInflowProb();
                }
            }
        }

        // having considered local constraints on the source and desitination patches, now consider
        // meso-scale density constraints on PlantTypeDistribution areas
        if (bAnyChance && localDensityLimitReached(iNewPos))
        {
            bAnyChance = false;
        }

        // -- Step 1c.3: if successful, create new plant and put in newPlants vector
        if (bAnyChance && (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < successProb))
        {
            Patch& newPatch = getPatch(iNewPos);

            // create a mutated version of the parent plant
            newPlants.emplace_back(pPlant, fNewPos, &newPatch, true);

            // and also update local density count
            incrementLocalDensityCount(iNewPos);
        }

        // -- Step 1c.4: if we've now reached the global limit on the number of new plants to
        //               produce, stop!
        if ((globalMax > 0) && (newPlants.size() > globalMax))
        {
            break;
        }
    }

    // -- Step 1d: empty plant vectors for all patches
    for (Patch& p : m_Patches)
    {
        p.killAllPlants();
    }

    // -- Step 1e: for each plant in newPlants, move it to appropriate Patch vec
    for (FloweringPlant& plant : newPlants)
    {
        plant.getPatch().addPlant(plant);
    }

    //////////////////////////////////////////////////////////////
    // Step 2: Reset all pollinators to their initial state
    for (Pollinator* pPollinator : m_AllPollinators)
    {
        pPollinator->reset();
    }

    //////////////////////////////////////////////////////////////
    // Step 3: Perform any other miscellaneous housekeeping at the start of a new generation

    m_bFlowerPtrVectorInitialised = false; // ensure m_AllFlowers will get refreshed
}


void Environment::initialiseLocalDensityCounts()
{
    const std::vector<PlantTypeDistributionConfig>& pdconfigs =
        ModelParams::getPlantTypeDistributionConfigs();

    for (const PlantTypeDistributionConfig& pdcfg : pdconfigs)
    {
        if (pdcfg.reproLocalDensityConstrained)
        {
            m_LocalDensityConstraints.emplace_back(pdcfg);
        }
    }
}

void Environment::resetLocalDensityCounts()
{
    for (LocalDensityConstraint& ldc : m_LocalDensityConstraints)
    {
        ldc.curPlants = 0;
    }
}

bool Environment::localDensityLimitReached(const iPos& newPatchPos) const
{
    for (const LocalDensityConstraint& ldc : m_LocalDensityConstraints)
    {
        if (ldc.posInArea(newPatchPos))
        {
            return (ldc.curPlants >= ldc.maxPlants);
        }
    }

    return false;
}

void Environment::incrementLocalDensityCount(const iPos& newPatchPos)
{
    for (LocalDensityConstraint& ldc : m_LocalDensityConstraints)
    {
        if (ldc.posInArea(newPatchPos))
        {
            ++(ldc.curPlants);
            return;
        }
    }
}


FlowerPtrVector& Environment::getAllFlowerPtrVector()
{
    if (!m_bFlowerPtrVectorInitialised)
    {
        m_AllFlowers.clear();

        for (Patch& patch : m_Patches)
        {
            // for each patch...
            PlantVector& plants = patch.getFloweringPlants();
            for (FloweringPlant &plant : plants)
            {
                // for each plant in patch...
                std::vector<Flower>& flowers = plant.getFlowers();
                for (Flower& flower : flowers)
                {
                    m_AllFlowers.push_back(&flower);
                }
            }
        }

        m_bFlowerPtrVectorInitialised = true;
    }

    return m_AllFlowers;
}
