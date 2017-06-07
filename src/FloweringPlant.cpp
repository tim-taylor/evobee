/**
 * @file
 *
 * Implementation of the FlowerPlant class
 */

#include <random>
#include <cassert>
#include "EvoBeeModel.h"
#include "FloweringPlant.h"

/**
 *
 */
FloweringPlant::FloweringPlant(const PlantTypeConfig& ptc, fPos pos) :
    m_species(ptc.species),
    m_iAntherPollen(ptc.antherInitPollen),
    m_iStigmaPollen(0),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity),
    m_iNectarRewardPerVisit(ptc.nectarReward),
    m_Position(pos),
    m_bHasLeaf(ptc.hasLeaf)
{
    assert(ptc.numFlowers > 0);

    if (m_bHasLeaf)
    {
        m_LeafReflectance.setMarkerPoint(ptc.leafMP);
    }

    for (int i=0; i<ptc.numFlowers; ++i)
    {
        MarkerPoint mp;

        if (ptc.flowerMPInitMin == ptc.flowerMPInitMax)
        {
            mp = ptc.flowerMPInitMin;
        }
        else
        {
            // pick a marker point at uniform random between the min and max values supplied
            std::uniform_int_distribution<MarkerPoint> dist(ptc.flowerMPInitMin, ptc.flowerMPInitMax);
            mp = dist(EvoBeeModel::m_sRngEngine);
        }

        ///@todo - for now, we are placing all flowers at the same position
        /// (the same position as the plant itself). If/when we start looking
        /// at plants with multiple flowers, we might want to change this
        m_Flowers.push_back( Flower(m_Position, mp, ptc.initTemp) );
    }
}

// Return the MarkerPoint of the specified flower
//
// @todo Asserts that the flower is valid, but should we throw an exception if not?
//
MarkerPoint FloweringPlant::getFlowerMarkerPoint(unsigned int flower)
{
    assert(flower < m_Flowers.size());
    return m_Flowers[flower].getMarkerPoint();
}

// Return the specified Flower
//
// @todo Asserts that the flower number is valid, but should we throw an exception if not?
//
Flower* FloweringPlant::getFlower(unsigned int flower)
{
    assert(flower < m_Flowers.size());
    return &(m_Flowers[flower]);   
}


float FloweringPlant::getDistance(const fPos& point) const
{
    return std::sqrt(getDistanceSq(point));
}

//
float FloweringPlant::getDistanceSq(const fPos& point) const
{
    return (((m_Position.x - point.x)*(m_Position.x - point.x)) + 
            ((m_Position.y - point.y)*(m_Position.y - point.y)));
}