/**
 * @file
 *
 * Declaration of the FloweringPlant class
 */

#ifndef _FLOWERINGPLANT_H
#define _FLOWERINGPLANT_H

#include <string>
#include <vector>
#include <map>
#include "PlantTypeConfig.h"
#include "PlantTypeDistributionConfig.h"
#include "ReflectanceInfo.h"
#include "Flower.h"
#include "Position.h"


class Patch;


/**
 * The FloweringPlant class ...
 */
class FloweringPlant {

public:
    /**
     * Constructor for creating a new plant at the beginning of a simulation
     */
    FloweringPlant(const PlantTypeConfig& typeConfig, const fPos& pos, Patch* pPatch);

    /**
     * Constructor for creating an offspring plant from a pollinated parent plant
     */
    FloweringPlant(const FloweringPlant* pParent, const fPos& pos, Patch* pPatch, bool mutate);

    // "Rule of 5" methods - https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
    FloweringPlant(const FloweringPlant& other);
    FloweringPlant(FloweringPlant&& other) noexcept;
    ~FloweringPlant() noexcept;
    FloweringPlant& operator= (const FloweringPlant& other);
    FloweringPlant& operator= (FloweringPlant&& other) noexcept;

    /**
     * Static method to register a species in the species map, given a species name.
     * If the name is not already in the map, then a new speciesId is assigned to it
     * and the id-name pair are added to the map.
     *
     * Returns the speciesId associated with the species (whether new or existing)
     */
    static unsigned int registerSpecies(const std::string& species);

    /**
     * Get the unique id of this plant
     */
    unsigned int getId() const {return m_id;}

    /**
     * Get the species id of this particular plant
     */
    unsigned int getSpeciesId() const {return m_SpeciesId;}

    /**
     * Get the species id of the species whose name is provided as a parameter
     */
    static unsigned int getSpeciesId(const std::string& name);

    /**
     * Return a string representing the species of this flower
     */
    const std::string& getSpecies() const;

    /**
     * Returns the MarkerPoint of the specified flower
     */
    MarkerPoint getFlowerMarkerPoint(unsigned int flower = 0);

    /**
     *
     */
    const fPos& getPosition() const {return m_Position;}

    /**
     * Returns a pointer to the specified flower
     */
    Flower* getFlower(unsigned int flower = 0);

    /**
     * Return a reference to this plant's vector of flowers
     */
    const std::vector<Flower>& getFlowers() const {return m_Flowers;}

    /**
     * Returns the distance between the plant and the specified point
     */
    float getDistance(const fPos& point) const;

    /**
     * Returns the square of the distance between the plant and the specified point
     */
    float getDistanceSq(const fPos& point) const;

    /**
     * Have any of this plant's flowers been pollinated?
     */
    bool pollinated() const {return m_bPollinated;}

    /**
     * Get a pointer to the patch to which this plant belongs.
     * Throws an exception if the patch info has not been initialised.
     */
    Patch& getPatch() const;

    /**
     * During reproduction, can seeds land anywhere in environment?
     */
    bool reproSeedDispersalGlobal();

    /**
     * During reproduction, if seed dispersal is not global, what is the dispersal
     * radius (expressed in environment units, so 1.0 equals one patch)
     */
    float reproSeedDispersalRadius();

    /**
     * A static method to construct the clogging map, which should be called once only,
     * at the start of a run when all params have been read from the config file
     */
    static void constructCloggingMap(std::vector<PlantTypeConfig>& ptcs);

    /**
     * Returns the list of all species ids of plants species whose flowers are clogged
     * by this species' pollen
     */
    const std::vector<unsigned int>& getCloggingSpeciesVec();

    /**
     * Returns the map of all known species, mapping speciesId to species name
     */
    static const std::map<unsigned int, std::string>& getSpeciesMap() {return m_sSpeciesMap;}

    /**
     * Determine whether the specified pollen is allowed to stick to the stigma of the specified destination
     * flower. It is allowed to do so if the pollen and the flower are from the same plant species, or if
     * the pollen is able to clog the destination plant species' stigma.
     */
    static bool pollenTransferToStigmaAllowed(const Flower* pPollenSource, const Flower* pDestination);

private:
    unsigned int            m_id;           ///< Unique ID number for this plant
    unsigned int            m_SpeciesId;    ///< ID number of the species of this plant
    fPos                    m_Position;
    std::vector<Flower>     m_Flowers;
    bool                    m_bHasLeaf;
    ReflectanceInfo         m_LeafReflectance;
    bool                    m_bPollinated;  ///< Have any of this plant's flowers been pollinated?
    Patch*                  m_pPatch;       ///< (non-owning) pointer to Patch in which this plant resides
    const PlantTypeConfig*  m_pPlantTypeConfig; ///< (non-owning) pointer to the Type Config object

    /**
     * Internal helper method for constructors and assignment operators
     */
    void copyCommon(const FloweringPlant& other) noexcept;

    /**
     * Set the plant's pollinated flag
     */
    void setPollinated(bool pollinated = true);

    /**
     * Record of next available unique ID number to be assigned to a new individual FloweringPlant
     */
    static unsigned int m_sNextFreeId;

    /**
     * Record of next available unique ID number to be assigned to a new species of FloweringPlant
     */
    static unsigned int m_sNextFreeSpeciesId;

    /**
     * A record of all currently known species IDs and their corresponding name
     */
    static std::map<unsigned int, std::string> m_sSpeciesMap;

    /**
     * A record of which species are clogged by each species
     */
    static std::map<unsigned int, std::vector<unsigned int>> m_sCloggingMap;

    /**
     * The Flower class is a friend of FloweringPlant
     */
    friend Flower;
};

#endif /* _FLOWERINGPLANT_H */
