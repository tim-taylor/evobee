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

    assert(inEnvironment(x,y));

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
                    patch.addPlant(pdcfg, *pPTC, pos);
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
FloweringPlant* Environment::findClosestFloweringPlant(const fPos& pos)
{
    FloweringPlant* pPlant = nullptr;
    float minDistSq = 99999.9;

    //std::cout << "Searching for plant closest to pos " << pos << std::endl;

    if (inEnvironment(pos))
    {
        int px = std::floor(pos.x);
        int py = std::floor(pos.y);
        for (int x = px - 1; x <= px + 1; ++x)
        {
            if (x >= 0 && x < m_iSizeX)
            {
                for (int y = py - 1; y <= py + 1; ++y)
                {
                    if (y >= 0 && y < m_iSizeY)
                    {
                        Patch& patch = getPatch(x,y);
                        PlantVector& plants = patch.getFloweringPlants();
                        for (FloweringPlant& plant : plants)
                        {
                            float distSq = plant.getDistanceSq(pos);
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
