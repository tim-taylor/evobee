/**
 * @file
 *
 * Declaration of the FloweringPlant class
 */

#ifndef _FLOWERINGPLANT_H
#define _FLOWERINGPLANT_H

#include <string>
#include "PlantTypeConfig.h"
#include "ReflectanceInfo.h"

/**
 * The FloweringPlant class ...
 */
class FloweringPlant {

public:
    FloweringPlant(const PlantTypeConfig& ptc, float x, float y);
    //~FloweringPlant() {}

private:
    std::string m_species;
    ReflectanceInfo m_Reflectance;
    int m_iAntherPollen;
    int m_iStigmaPollen;
    int m_iStigmaMaxPollenCapacity;
    int m_iNectarRewardPerVisit;
    float m_posX;
    float m_posY;
};

#endif /* _FLOWERINGPLANT_H */
