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

        m_Flowers.push_back( Flower(m_Position, mp, ptc.initTemp) );
    }
}

/**
 * Return the MarkerPoint of the specified flower
 * @todo for the time being we just assume one flower - need to change this
 * when FloweringPlant actually has a list of Flowers
 */
MarkerPoint FloweringPlant::getFlowerMarkerPoint(int flower)
{
    assert(flower >= 0);
    assert(flower < m_Flowers.size());
    return m_Flowers[flower].getMarkerPoint();
}