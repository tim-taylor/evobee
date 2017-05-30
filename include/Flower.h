/**
 * @file
 *
 * Declaration of the Flower class
 */

#ifndef _FLOWER_H
#define _FLOWER_H

#include "ReflectanceInfo.h"
#include "Position.h"

/**
 * The Flower class ...
 */
class Flower {

public:
    Flower(fPos pos, MarkerPoint mp, float temp) :
        m_Position(pos),
        m_Reflectance(mp),
        m_fTemp(temp)
    {}

    ///
    float getTemp() const {return m_fTemp;}

    ///
    MarkerPoint getMarkerPoint() const {return m_Reflectance.getMarkerPoint();}

private:
    fPos m_Position;
    ReflectanceInfo m_Reflectance;
    float m_fTemp;
};

#endif /* _FLOWER_H */
