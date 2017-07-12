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
    FloweringPlant(const PlantTypeConfig& typeConfig, const fPos& pos, const Patch* pPatch);

    /**
     * Constructor for creating an offspring plant from a pollinated parent plant
     */
    FloweringPlant(const FloweringPlant* pParent, const fPos& pos, const Patch* pPatch, bool mutate);

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
    const Patch& getPatch() const;


private:
    unsigned int            m_id;           ///< Unique ID number for this plant
    unsigned int            m_SpeciesId;    ///< ID number of the species of this plant
    fPos                    m_Position;
    std::vector<Flower>     m_Flowers;
    bool                    m_bHasLeaf;
    ReflectanceInfo         m_LeafReflectance;
    bool                    m_bPollinated;  ///< Have any of this plant's flowers been pollinated?
    const Patch*            m_pPatch;       ///< (non-owning) pointer to Patch in which this plant resides
    const PlantTypeConfig*  m_pPlantTypeConfig; ///< (non-owning) pointer to the Type Config object

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
     * The Flower class is a friend of FloweringPlant
     */
    friend Flower;
};

#endif /* _FLOWERINGPLANT_H */
