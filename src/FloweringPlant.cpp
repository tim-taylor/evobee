/**
 * @file
 *
 * Implementation of the FlowerPlant class
 */

#include <random>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <exception>
#include <regex>
#include "EvoBeeModel.h"
#include "FloweringPlant.h"
#include "Patch.h"
#include "ModelParams.h"


// Initialise static members of class
unsigned int FloweringPlant::m_sNextFreeId = 1;
unsigned int FloweringPlant::m_sNextFreeSpeciesId = 1;
std::map<unsigned int, std::string> FloweringPlant::m_sSpeciesMap;
std::map<unsigned int, std::vector<unsigned int>> FloweringPlant::m_sCloggingMap;
bool FloweringPlant::m_sbCloggingAll = false;
bool FloweringPlant::m_sbCloggingNone = false;


// Create a brand new plant at the start of the simulation from the specified config
FloweringPlant::FloweringPlant(const PlantTypeConfig& typeConfig,
                               const fPos& pos,
                               Patch* pPatch) :
    m_id(m_sNextFreeId++),
    m_Position(pos),
    m_bHasLeaf(typeConfig.hasLeaf),
    m_bPollinated(false),
    m_pPatch(pPatch)
{
    assert(typeConfig.numFlowers > 0);

    m_pPlantTypeConfig = &typeConfig;

    // Get the corresponding speciesID for the given species name (and if we have not
    // come across this species before, assign a new speciesID and add it to the species map)
    m_SpeciesId = registerSpecies(typeConfig.species);

    if (m_bHasLeaf)
    {
        m_LeafReflectance.setMarkerPoint(typeConfig.leafMP);
    }

    for (int i=0; i < typeConfig.numFlowers; ++i)
    {
        switch (ModelParams::getColourSystem())
        {
            case ColourSystem::REGULAR_MARKER_POINTS:
            {
                MarkerPoint mp;

                if (typeConfig.flowerMPInitMin == typeConfig.flowerMPInitMax)
                {
                    mp = typeConfig.flowerMPInitMin;
                }
                else
                {
                    // pick a marker point at uniform random between the min and max values supplied
                    assert(typeConfig.flowerMPInitStep > 0);
                    unsigned int mpRange = typeConfig.flowerMPInitMax - typeConfig.flowerMPInitMin;
                    unsigned int numPointsMinusOne = mpRange / typeConfig.flowerMPInitStep;
                    std::uniform_int_distribution<unsigned int> dist(0, numPointsMinusOne);
                    unsigned int rndIdx = dist(EvoBeeModel::m_sRngEngine);
                    mp = typeConfig.flowerMPInitMin + (rndIdx * typeConfig.flowerMPInitStep);
                    assert(mp >= typeConfig.flowerMPInitMin);
                    assert(mp <= typeConfig.flowerMPInitMax);
                }

                /// Note: for now, we are placing all flowers at the same position
                /// (the same position as the plant itself). If/when we start looking
                /// at plants with multiple flowers, we might want to change this
                m_Flowers.emplace_back(this, typeConfig, m_Position, mp);
                break;
            }
            case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
            {
                m_Flowers.emplace_back(this, typeConfig, m_Position, typeConfig.flowerMPInitMin);
                break;
            }
            default:
            {
                throw std::runtime_error("Unknown ColourSystem encountered in FloweringPlant constructor");
            }
        }
    }
}


// Create an offspring plant from the specified parent
FloweringPlant::FloweringPlant( const FloweringPlant* pParent,
                                const fPos& pos,
                                Patch* pPatch,
                                bool  mutate ) :
    m_id(m_sNextFreeId++),
    m_bPollinated(false)
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


// Static method to register a species in the species map, given a species name
// If the name is not already in the map, then a new speciesId is assigned to it
// and it is added to the map.
//
// Returns the speciesId associated with the species (whether new or existing)
//
unsigned int FloweringPlant::registerSpecies(const std::string& species)
{
    unsigned int speciesId = 0;

    // Do we already have a record of this plant species?
    auto it = std::find_if( m_sSpeciesMap.begin(),
                            m_sSpeciesMap.end(),
                            [species](const std::pair<unsigned int, std::string> & pair)
                                {return pair.second == species;} );
    if (it != m_sSpeciesMap.end())
    {
        speciesId = it->first;
    }
    else
    {
        // this is a new species, so select a new species id to use, and also
        // insert of record of the new id and species name into m_sSpeciesMap
        speciesId = m_sNextFreeSpeciesId++;
        m_sSpeciesMap[speciesId] = species;

        if (ModelParams::verbose())
        {
            std::cout << "Adding new plant species to map: id=" << speciesId << ", name=" <<
                species << std::endl;
        }
    }

    return speciesId;
}


// return the string description of this species
const std::string& FloweringPlant::getSpecies() const
{
    // we assume that the string corresponding to the species ID exists in the
    // map, because this is taken care of in the constructor
    return m_sSpeciesMap[m_SpeciesId];
}


// Static method to return species id of the species whose name is given as a param.
unsigned int FloweringPlant::getSpeciesId(const std::string& name)
{
    if (name == "nogo")
    {
        return 0;
    }
    else
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
}


// Return the MarkerPoint of the specified flower
//
// @todo Asserts that the flower is valid, but should we throw an exception if not?
//
/* -- use getFlowerCharacteristicWavelength instead!
MarkerPoint FloweringPlant::getFlowerMarkerPoint(unsigned int flower)
{
    assert(flower < m_Flowers.size());
    return m_Flowers[flower].getMarkerPoint();
}
*/


// Return the MarkerPoint of the specified flower
//
// @todo Asserts that the flower is valid, but should we throw an exception if not?
//
MarkerPoint FloweringPlant::getFlowerCharacteristicWavelength(unsigned int flower)
{
    assert(flower < m_Flowers.size());
    return m_Flowers[flower].getCharacteristicWavelength();
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


float FloweringPlant::reproSeedDispersalRadiusStdDev()
{
    if (m_pPlantTypeConfig != nullptr)
    {
        return m_pPlantTypeConfig->reproSeedDispersalRadiusStdDev;
    }
    else
    {
        throw std::runtime_error("FloweringPlant::reproSeedDispersalRadiusStdDev no config defined");
        return 1.0;
    }
}


void FloweringPlant::constructCloggingMap(std::vector<PlantTypeConfig>& ptcs)
{
    assert(m_sCloggingMap.empty());

    for (PlantTypeConfig& ptc : ptcs)
    {
        unsigned int thisSpeciesId = getSpeciesId(ptc.species);

        if (ptc.pollenCloggingSpecies.empty())
        {
            ptc.pollenCloggingAll = false;
            ptc.pollenCloggingPartial = false;

            // this species does not clog anything, so add an empty entry
            // for this species to the clogging map
            std::vector<unsigned int> cloggingSpIdVec;
            m_sCloggingMap[thisSpeciesId] = cloggingSpIdVec;
        }
        else if (ptc.pollenCloggingSpecies == "all")
        {
            ptc.pollenCloggingAll = true;
            ptc.pollenCloggingPartial = false;

            // this species clogs everything, so add the ids of all other species to
            // the entry for this species in the map
            std::vector<unsigned int> cloggingSpIdVec;
            for (auto& entry : m_sSpeciesMap)
            {
                if (entry.first != thisSpeciesId)
                {
                    cloggingSpIdVec.push_back(entry.first);
                }
            }
            m_sCloggingMap[thisSpeciesId] = cloggingSpIdVec;
        }
        else
        {
            ptc.pollenCloggingAll = false;
            ptc.pollenCloggingPartial = true;

            // this species clogs some other species, but not necessarily all
            // so we need to add specific ids to this species' entry in the map

            // first build a vector of the name of each species that is clogged by this one
            // (ptc.pollenCloggingSpecies contains the names in a single comma-separated string)
            std::regex delim(",\\s+"); // comma followed by optional whitespace
            std::vector<std::string> cloggingSpNameVec {
                std::sregex_token_iterator(ptc.pollenCloggingSpecies.begin(), ptc.pollenCloggingSpecies.end(), delim, -1),
                std::sregex_token_iterator()
            };

            // now for each species name in the vector, get the corresponding speciesId and add it to
            // the vector to be used in the clogging map
            std::vector<unsigned int> cloggingSpIdVec;
            for (std::string& cloggingSpName : cloggingSpNameVec)
            {
                if (ModelParams::verbose())
                {
                    std::cout << "Species " << ptc.species << " clogs species " << cloggingSpName << std::endl;
                }

                cloggingSpIdVec.push_back(getSpeciesId(cloggingSpName));
            }

            m_sCloggingMap[thisSpeciesId] = cloggingSpIdVec;

        }
    }

    // Calculate the values of m_sbCloggingAll and m_sbCloggingNone
    unsigned int numSpecies = m_sCloggingMap.size();
    unsigned int numEmpty = 0;
    unsigned int numFull = 0;
    for (auto& entry : m_sCloggingMap)
    {
        if (entry.second.empty())
        {
            // this species does not clog any other species
            ++numEmpty;
        }
        else if (entry.second.size() >= numSpecies - 1)
        {
            // this species clogs all other species
            ++numFull;
        }
    }
    m_sbCloggingAll = (numFull == numSpecies);
    m_sbCloggingNone = (numEmpty == numSpecies);
}


const std::vector<unsigned int>& FloweringPlant::getCloggingSpeciesVec()
{
    auto it = m_sCloggingMap.find(m_SpeciesId);

    if (it == m_sCloggingMap.end())
    {
        std::stringstream msg;
        msg << "Error: request for clogging species map of unknown species " << m_SpeciesId;
        throw std::runtime_error(msg.str());
    }
    else
    {
        return it->second;
    }
}


// Determine whether the specified pollen is allowed to stick to the stigma of the specified destination
// flower. It is allowed to do so if the pollen and the flower are from the same plant species, or if
// the pollen is able to clog the destination plant species' stigma.
bool FloweringPlant::pollenTransferToStigmaAllowed(const Flower* pPollenSource, const Flower* pDestination)
{
    unsigned int pollenSpeciesId = pPollenSource->getSpeciesId();
    unsigned int flowerSpeciesId = pDestination->getSpeciesId();

    if (pollenSpeciesId == flowerSpeciesId)
    {
        return true;
    }
    else if (pPollenSource->pollenCloggingAll())
    {
        return true;
    }
    else if (!pPollenSource->pollenCloggingPartial())
    {
        return false;
    }
    else
    {
        auto it = m_sCloggingMap.find(pollenSpeciesId);

        if (it == m_sCloggingMap.end())
        {
            std::stringstream msg;
            msg << "Error: request for clogging species map of unknown species " << pollenSpeciesId;
            throw std::runtime_error(msg.str());
        }
        else
        {
            const std::vector<unsigned int>& cloggedSp = it->second;
            auto it = std::find(cloggedSp.begin(), cloggedSp.end(), flowerSpeciesId);
            if (it != cloggedSp.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}