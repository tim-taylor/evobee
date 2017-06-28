/**
 * @file
 *
 * Implementation of the Patch class
 */

#include <iostream>
#include <sstream>
#include "Environment.h"
#include "Patch.h"
#include "FloweringPlant.h"


Patch::Patch(Environment* pEnv, int posIdx, MarkerPoint mp, float temp) :
    m_pEnv(pEnv),
    m_fTemp(temp),
    m_posIdx(posIdx),
    m_BackgroundReflectance(mp),
    m_bReproConstraintsSetExplicitly(false),
    m_bSeedOutflowAllowed(true),
    m_bSeedOutflowRestricted(false),
    m_bSeedOutflowProb(1.0),
    m_bRefuge(false),
    m_iRefugeNativeSpecesId(0),
    m_fRefugeAlienInflowProb(1.0)
{
    m_Position = m_pEnv->getPatchCoordsFromIdx(posIdx);
}


void Patch::addPlant(const PlantTypeDistributionConfig& distConfig,
                     const PlantTypeConfig& typeConfig, fPos pos)
{
    std::cout << "Adding plant of species " << typeConfig.species << " to Patch [" << 
        m_Position << "] at coordinates (" << pos << ")" << std::endl;

    m_FloweringPlants.emplace_back(distConfig, typeConfig, pos);
}


void Patch::setReproConstraints(const PlantTypeDistributionConfig& pdcfg)
{
    if (m_bReproConstraintsSetExplicitly)
    {
        std::stringstream msg;
        msg << "Attempt to set multiple sets of reproduction constraints on patch "
            << m_Position << ". Overlapping PlantTypeDistribution areas with"
            << "constraints on seed flow?";
        throw std::runtime_error(msg.str());
    }

    m_bSeedOutflowAllowed = pdcfg.seedOutflowAllowed;
    m_bSeedOutflowRestricted = pdcfg.seedOutflowRestricted;
    m_bSeedOutflowProb = pdcfg.seedOutflowProb;
    m_bRefuge = pdcfg.refuge;
    m_iRefugeNativeSpecesId = FloweringPlant::getSpeciesId(pdcfg.species);
    m_fRefugeAlienInflowProb = pdcfg.refugeAlienInflowProb;

    m_bReproConstraintsSetExplicitly = true;
}