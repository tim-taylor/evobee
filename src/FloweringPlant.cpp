/**
 * @file
 *
 * Implementation of the FlowerPlant class
 */

#include <random>
#include "EvoBeeModel.h"
#include "FloweringPlant.h"

FloweringPlant::FloweringPlant(const PlantTypeConfig& ptc, float x, float y) :
    m_species(ptc.species),
    m_iAntherPollen(ptc.antherInitPollen),
    m_iStigmaPollen(0),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity),
    m_iNectarRewardPerVisit(ptc.nectarReward),
    m_posX(x),
    m_posY(y)
{
    if (ptc.flowerMPInitMin == ptc.flowerMPInitMax)
    {
        m_Reflectance.setMarkerPoint(ptc.flowerMPInitMin);
    }
    else
    {
        // pick a marker point at uniform random between the min and max values supplied
        std::uniform_int_distribution<MarkerPoint> dist(ptc.flowerMPInitMin, ptc.flowerMPInitMax);
        m_Reflectance.setMarkerPoint(dist(EvoBeeModel::m_sRngEngine));    
    }
}