/**
 * @file
 *
 * Implementation of the Environment class
 */

#include <cassert>
#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include <cmath>
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


Environment::Environment(EvoBeeModel* pModel) :
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

    // Initialise Hives
    const std::vector<HiveConfig> & hconfigs = ModelParams::getHiveConfigs();
    for (const HiveConfig& hconfig : hconfigs)
    {
        // Use the AbstractHive::makeHive factory method to make a Hive of the right type
        m_Hives.push_back( AbstractHive::makeHive(this, hconfig) );
    }

    // Initialise Plants
    initialisePlants();

    // Initialise internal book-keeping for local density limits during plant reproduction
    initialiseLocalDensityCounts();

    std::cout << "Initialised Environment with " << m_Patches.size() << " patches" << std::endl;
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
 * many plants in total should be created, it then randomly assigned positions for each
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
        // first find the corresponding PlantTypeConfig
        const PlantTypeConfig* pPTC = ModelParams::getPlantTypeConfig(pdcfg.species);
        if (pPTC == nullptr)
        {
            throw std::runtime_error("Unknown plant species '" + pdcfg.species +
                "' specified in config file");
        }

        // calculate some basic values associated with the requested distribution
        int w = std::max(1, 1 + pdcfg.areaBottomRight.x - pdcfg.areaTopLeft.x);
        int h = std::max(1, 1 + pdcfg.areaBottomRight.y - pdcfg.areaTopLeft.y);
        int area = w * h;
        int numPlants = (int)((float)area * pdcfg.density);
        std::uniform_real_distribution<float> distW(0.0, w);
        std::uniform_real_distribution<float> distH(0.0, h);

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
    static std::uniform_real_distribution<float> distX((float)topleft.x, (float)(bottomright.x+1));
    static std::uniform_real_distribution<float> distY((float)topleft.y, (float)(bottomright.y+1));
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


// Search for plants in the local patch and its 8 closest neighbours,
// and return a pointer to the closest plant found, or nullptr if none found
//
FloweringPlant* Environment::findClosestFloweringPlant(const fPos& fpos)
{
    FloweringPlant* pPlant = nullptr;
    float minDistSq = 9999999.9;

    //std::cout << "Searching for plant closest to pos " << pos << std::endl;

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
                        Patch& patch = getPatch(x,y);
                        PlantVector& plants = patch.getFloweringPlants();
                        for (FloweringPlant& plant : plants)
                        {
                            float distSq = plant.getDistanceSq(fpos);
                            if (distSq < minDistSq)
                            {
                                //std::cout << "Plant " << &plant << " is at distance " << distSq << std::endl;
                                minDistSq = distSq;
                                pPlant = &plant;
                            }
                        }
                    }
                }
            }
        }
    }

    return pPlant;
}


/**
 * Initialise a new generation:
 * 1. Construct a new generation of plants based upon those successfully
 *    pollinated in the previous generation, taking into acconut any refuges
 *    and/or restrictions to seed flow. 
 * 2. Reset all pollinators to initial state.
 * 3, Perform other housekeeping tasks.
 */
void Environment::initialiseNewGeneration()
{
    // generate a new populations of plants based upon pollinated plants from
    // the previous generation, then delete all plants from previous generation
    //
    ///@todo we also need to log this info if necessary
    //
    /*
    overall goal:
        replace all plants (stored in individual Patches) with a
        new generation of plants
    assumptions:
        If PTD area is refuge or seedOutflow is restricted (outflow prob < 1.0), that
            area is not allowed to overlap with any other area
        (see implementation details below)
    constraints:
        plant type distributions constraints:
            refuges (restriction on incoming alien speices) (isRefuge/prob)
            seed outflow (allowed/prob)
    which plants reproduce?
        flower is pollinated
        possibilities:
            max limits defined by:
                fixed number of plants in each PTD area (defined by density param)
                fixed number across whole environment
    approach:
        create a vector of all pollinated plants and shuffle it
        create an empty vector to store new generation
        for each plant in vector (up to global max repro num for env)
            consider a nearby position in which to reproduce
            determine prob that it will successfully reproduce in that position, taking into acccount 
                seed outflow prob of current patch
                seed inflow prob (refuge incoming) of destination
                any local PTDConfig density constraints (stored in ModelParams)
            if successful, create new plant and put in newgenvec
        empty plant vectors for all patches
        for each plant in newgenvec, move plant to appropriate Patch vec
    implementation details:
        each Patch needs 
            outflow prob (and allowed/restricted flags? corresponding to p>0, p<1)
            isRefuge flag
                refugeNativeSpecies id
                alienInflowProb
        We can deal with constraints on overlapping PTD areas (see assumptions above) by 
            thowing an exception if there is an attempt to set these Patch probs/flags
            muliple times for any single Patch
    New params required:
        Environment -> repro-constraint-max-density-global
        PTDConfig -> repro-constraint-max-density-area (if isRefuge or outflowProb=0)
    */

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
            PlantVector & fplants = p.getFloweringPlants();
            for (FloweringPlant & fplant : fplants)
            {
                if (fplant.pollinated())
                {
                    pollinatedPlantPtrs.push_back(&fplant);
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
        // -- Step 1c.1: consider a nearby position in which to reproduce
        ///@todo at this stage we've got a distance between 0-1 - maybe need a wider
        ///      distribution, or maybe even just have a fixed diatance of 1?
        float distance = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine);
        float heading  = EvoBeeModel::m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);
        fPos delta   { distance*std::cos(heading), distance*std::sin(heading) };
        fPos fCurPos { pPlant->getPosition() };
        fPos fNewPos { fCurPos + delta };

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
                // new plant is still in environment but in a different patch to old one
                const Patch& curPatch = pPlant->getPatch();

                // first consider our chances of successfully leaving the current patch
                if (!curPatch.seedOutflowAllowed())
                {
                    // no seed outflow is allowed from this patch!
                    bAnyChance = false;
                }
                else if (curPatch.seedOutflowRestricted())
                {
                    // seed outflow is allowed at a restricted rate
                    // (multiply the prob of doing this with our currently calculated prob)
                    successProb *= curPatch.getSeedOutflowProb();
                }

                // now consider chances of succesfully moving into the new patch
                if (bAnyChance)
                {
                    Patch& newPatch = getPatch(iNewPos);
                    if (newPatch.refuge() && (newPatch.getRefugeNativeSpeciesId() != pPlant->getSpeciesId()))
                    {
                        // trying to move into a refuge for a different plant species
                        // (multiply the prob of doing this with our currently calculated prob)
                        successProb *= newPatch.getRefugeAlienInflowProb();
                    }
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
    //

    //////////////////////////////////////////////////////////////
    // Step 3: anything else to do? I don't think so...
    ///@todo (maybe)    
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