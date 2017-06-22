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

/**
 * The FloweringPlant class ...
 */
class FloweringPlant {

public:
    FloweringPlant(const PlantTypeDistributionConfig& distConfig,
                   const PlantTypeConfig& typeConfig, fPos pos);

    /**
     *
     */
    unsigned int getId() const {return m_id;}

    /**
     *
     */
    unsigned int getSpeciesId() const {return m_SpeciesId;}

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


private:
    unsigned int        m_id;           ///< Unique ID number for this plant
    unsigned int        m_SpeciesId;    ///< ID number of the species of this plant
    fPos                m_Position;
    std::vector<Flower> m_Flowers;
    bool                m_bHasLeaf;
    ReflectanceInfo     m_LeafReflectance;

    //const PlantTypeDistributionConfig& ...;

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
};

#endif /* _FLOWERINGPLANT_H */
