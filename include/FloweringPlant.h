/**
 * @file
 *
 * Declaration of the FloweringPlant class
 */

#ifndef _FLOWERINGPLANT_H
#define _FLOWERINGPLANT_H

#include <string>
#include <vector>
#include "PlantTypeConfig.h"
#include "ReflectanceInfo.h"
#include "Flower.h"
#include "Position.h"

/**
 * The FloweringPlant class ...
 */
class FloweringPlant {

public:
    FloweringPlant(const PlantTypeConfig& ptc, fPos pos);
    //~FloweringPlant() {}

    /**
     * Returns the MarkerPoint of the specified flower
     */
    MarkerPoint getFlowerMarkerPoint(unsigned int flower = 0);

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
     * Return a string representing the species of this flower
     */
    const std::string& getSpecies() const {return m_Species;}

private:
    std::string m_Species;
    fPos m_Position;
    std::vector<Flower> m_Flowers;
    bool m_bHasLeaf;
    ReflectanceInfo m_LeafReflectance;
};

#endif /* _FLOWERINGPLANT_H */
