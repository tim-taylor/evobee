/**
 * @file
 *
 * Declaration of the Flower and Pollen classes
 */

#ifndef _FLOWER_H
#define _FLOWER_H

#include "ReflectanceInfo.h"
#include "Position.h"
#include "PlantTypeConfig.h"

class Flower;


/**
 * The Pollen class ...
 */
 struct Pollen {
     Pollen() : pSource(nullptr), numLandings(0) {}
     Pollen(const Flower* pFlower) : pSource(pFlower), numLandings(0) {}

     const Flower* pSource;
     int numLandings;
 };


/**
 * The Flower class ...
 */
class Flower {

public:
    Flower(const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp);

    ///
    float getTemp() const {return m_fTemp;}

    ///
    MarkerPoint getMarkerPoint() const {return m_Reflectance.getMarkerPoint();}

private:
    fPos        m_Position;                 ///< Spatial position of the flower
    ReflectanceInfo m_Reflectance;          ///< Flower reflectance info
    bool        m_bPollinated;              ///< Is the flower pollinated?
    int         m_iAntherPollen;            ///< Amount of collectable pollen remaining
    const int   m_iAntherPollenLossPerVisit;///< Number of pollen grains deposited on a pollinator per visit
    int         m_iStigmaPollen;            ///< Amount of pollen currently deposited on stigma
    const int   m_iStigmaMaxPollenCapacity; ///< Maximum amount of pollen the stigma can carry
    /*int m_iNectarRewardPerVisit;          ///< Amount of reward given to a pollinator (CURRENTLY UNUSED)*/
    float       m_fTemp;                    ///< Current temperature of flower
};

#endif /* _FLOWER_H */
