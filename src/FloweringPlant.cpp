/**
 * @file
 *
 * Implementation of the FlowerPlant class
 */

#include <random>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <exception>
#include "EvoBeeModel.h"
#include "FloweringPlant.h"
#include "Patch.h"


// Initialise static members of class
unsigned int FloweringPlant::m_sNextFreeId = 1;
unsigned int FloweringPlant::m_sNextFreeSpeciesId = 1;
std::map<unsigned int, std::string> FloweringPlant::m_sSpeciesMap;


// Create a brand new plant at the start of the simulation from the specified config
FloweringPlant::FloweringPlant(const PlantTypeConfig& typeConfig,
                               const fPos& pos,
                               Patch* pPatch) :
    m_id(m_sNextFreeId++),
    m_Position(pos),
    m_bHasLeaf(typeConfig.hasLeaf),
    m_pPatch(pPatch)
{
    assert(typeConfig.numFlowers > 0);

    m_pPlantTypeConfig = &typeConfig;

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


// Create an offspring plant from the specified parent
FloweringPlant::FloweringPlant( const FloweringPlant* pParent, 
                                const fPos& pos,
                                Patch* pPatch,
                                bool  mutate ) :
    m_id(m_sNextFreeId++)
{
    assert(pParent != nullptr);
    assert(pPatch != nullptr);

    m_SpeciesId = pParent->m_SpeciesId;
    m_pPlantTypeConfig = pParent->m_pPlantTypeConfig;
    m_pPatch = pPatch;
    m_Position = pos;
    m_bHasLeaf = pParent->m_bHasLeaf;
    m_LeafReflectance = pParent->m_LeafReflectance;
    for (const Flower& parentFlower : pParent->m_Flowers)
    {
        ///@todo take note of the mutate param, and maybe change some values,
        // particularly the Marker Point/Reflectance info!
        const ReflectanceInfo& reflectance = parentFlower.getReflectanceInfo();
        m_Flowers.emplace_back(this, parentFlower, m_Position, reflectance);
    }
}


// copy constructor
FloweringPlant::FloweringPlant(const FloweringPlant& other) :
    m_id(m_sNextFreeId++),          // for copy constructor we assign a new id
    m_SpeciesId(other.m_SpeciesId),
    m_Position(other.m_Position),
    m_Flowers(other.m_Flowers),
    m_bHasLeaf(other.m_bHasLeaf),
    m_LeafReflectance(other.m_LeafReflectance),
    m_bPollinated(other.m_bPollinated),
    m_pPatch(other.m_pPatch),
    m_pPlantTypeConfig(other.m_pPlantTypeConfig)
{
    assert(false); // it's probable that we don't want to be here!

    for (Flower& flower : m_Flowers) 
    {
        flower.setPlant(this);
    }
}


// move constructor
FloweringPlant::FloweringPlant(FloweringPlant&& other) noexcept :
    m_id(other.m_id),                // for move constructor we keep the same id
    m_SpeciesId(other.m_SpeciesId),
    m_Position(other.m_Position),
    m_Flowers(std::move(other.m_Flowers)),
    m_bHasLeaf(other.m_bHasLeaf),
    m_LeafReflectance(other.m_LeafReflectance),
    m_bPollinated(other.m_bPollinated),
    m_pPatch(other.m_pPatch),
    m_pPlantTypeConfig(other.m_pPlantTypeConfig)
{
    for (Flower& flower : m_Flowers) 
    {
        flower.setPlant(this);
    }
}


// destructor
FloweringPlant::~FloweringPlant() noexcept
{
}


// copy assignment operator
FloweringPlant& FloweringPlant::operator= (const FloweringPlant& other)
{
    copyCommon(other);
    m_id = m_sNextFreeId++; // for copy assignment we assign a new id
    return *this;
}


// move assignment operator
FloweringPlant& FloweringPlant::operator= (FloweringPlant&& other) noexcept
{
    copyCommon(other);
    m_id = other.m_id;      // for move assignment we keep the same id
    other.m_id = 0;
    return *this;
}


// helper method used by copy/move assignment operators
void FloweringPlant::copyCommon(const FloweringPlant& other) noexcept
{
    m_SpeciesId = other.m_SpeciesId;
    m_Position = other.m_Position;
    m_Flowers = other.m_Flowers;
    for (Flower& flower : m_Flowers) 
    {
        flower.setPlant(this);
    }
    m_bHasLeaf = other.m_bHasLeaf;
    m_LeafReflectance = other.m_LeafReflectance;
    m_bPollinated = other.m_bPollinated;
    m_pPatch = other.m_pPatch;
    m_pPlantTypeConfig = other.m_pPlantTypeConfig;
}


// return the string description of this species
const std::string& FloweringPlant::getSpecies() const
{
    // we assume that the string corresponding to the species ID exists in the
    // map, because this is taken care of in the constructor
    return m_sSpeciesMap[m_SpeciesId];
}


// return species id of the species whose name is given as a param (static method)
unsigned int FloweringPlant::getSpeciesId(const std::string& name)
{
    auto it = std::find_if( m_sSpeciesMap.begin(),
                            m_sSpeciesMap.end(),
                            [name](const std::pair<unsigned int, std::string>& pair)
                                {return (pair.second == name);} );

    if (it == m_sSpeciesMap.end())
    {
        throw std::runtime_error("Unable to find id for plant species " + name);
    }

    return it->first;
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


float FloweringPlant::getDistanceSq(const fPos& point) const
{
    return (((m_Position.x - point.x)*(m_Position.x - point.x)) + 
            ((m_Position.y - point.y)*(m_Position.y - point.y)));
}


void FloweringPlant::setPollinated(bool pollinated)
{
    m_bPollinated = pollinated;
}


Patch& FloweringPlant::getPatch() const
{
    if (m_pPatch == nullptr)
    {
        throw std::runtime_error("Attempt to query uninitialised Patch info from a FloweringPlant.");
    }
    else
    {
        return *m_pPatch;
    }
}


bool FloweringPlant::reproSeedDispersalGlobal()
{
    if (m_pPlantTypeConfig != nullptr)
    {
        return m_pPlantTypeConfig->reproSeedDispersalGlobal;
    }
    else
    {
        throw std::runtime_error("FloweringPlant::reproSeedDispersalGlobal no config defined");
        return false;
    }
}


float FloweringPlant::reproSeedDispersalRadius()
{
    if (m_pPlantTypeConfig != nullptr)
    {
        return m_pPlantTypeConfig->reproSeedDispersalRadius;
    }
    else
    {
        throw std::runtime_error("FloweringPlant::reproSeedDispersalRadius no config defined");
        return 1.0;
    }
}
