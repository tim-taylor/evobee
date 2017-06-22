/**
 * @file
 *
 * Implementation of the Patch class
 */

#include <iostream>
#include "Environment.h"
#include "Patch.h"


Patch::Patch(Environment* pEnv, int posIdx, MarkerPoint mp, float temp) :
    m_pEnv(pEnv),
    m_fTemp(temp),
    m_posIdx(posIdx),
    m_BackgroundReflectance(mp)
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