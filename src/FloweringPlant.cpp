/**
 * @file
 *
 * Implementation of the FlowerPlant class
 */

#include <random>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "EvoBeeModel.h"
#include "FloweringPlant.h"


// Initialise static members of class
unsigned int FloweringPlant::m_sNextFreeId = 1;
unsigned int FloweringPlant::m_sNextFreeSpeciesId = 1;
std::map<unsigned int, std::string> FloweringPlant::m_sSpeciesMap;


/**
 *
 */
FloweringPlant::FloweringPlant(const PlantTypeDistributionConfig& distConfig,
                               const PlantTypeConfig& typeConfig, fPos pos) :
    m_id(m_sNextFreeId++),
    m_Position(pos),
    m_bHasLeaf(typeConfig.hasLeaf)
{
    ///@todo do something with distConfig...

    assert(typeConfig.numFlowers > 0);

    // Do we already have a record of this plant species?
    auto it = std::find_if( m_sSpeciesMap.begin(),
                            m_sSpeciesMap.end(),
                            [typeConfig](const std::pair<unsigned int, std::string> & pair)
                                {return pair.second == typeConfig.species;} );
    if (it != m_sSpeciesMap.end())
    {
        // species already know, so assign the associated species id to the plant
        m_SpeciesId = it->first;
    }
    else
    {
        // this is a new species, so select a new species id to use, and also
        // insert of record of the new id and species name into m_sSpeciesMap
        m_SpeciesId = m_sNextFreeSpeciesId++;
        m_sSpeciesMap[m_SpeciesId] = typeConfig.species;

        std::cout << "Adding new plant species to map: id=" << m_SpeciesId << ", name=" <<
            typeConfig.species << std::endl;
    }

    if (m_bHasLeaf)
    {
        m_LeafReflectance.setMarkerPoint(typeConfig.leafMP);
    }

    for (int i=0; i < typeConfig.numFlowers; ++i)
    {
        MarkerPoint mp;

        if (typeConfig.flowerMPInitMin == typeConfig.flowerMPInitMax)
        {
            mp = typeConfig.flowerMPInitMin;
        }
        else
        {
            // pick a marker point at uniform random between the min and max values supplied
            std::uniform_int_distribution<MarkerPoint> dist(
                typeConfig.flowerMPInitMin, typeConfig.flowerMPInitMax);
            mp = dist(EvoBeeModel::m_sRngEngine);
        }

        ///@todo - for now, we are placing all flowers at the same position
        /// (the same position as the plant itself). If/when we start looking
        /// at plants with multiple flowers, we might want to change this
        m_Flowers.emplace_back(this, typeConfig, m_Position, mp);
    }
}


// return the string description of this species
const std::string& FloweringPlant::getSpecies() const
{
    // we assume that the string corresponding to the species ID exists in the
    // map, because this is taken care of in the constructor
    return m_sSpeciesMap[m_SpeciesId];
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