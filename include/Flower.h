/**
 * @file
 *
 * Declaration of the Flower class
 */

#ifndef _FLOWER_H
#define _FLOWER_H

#include "ReflectanceInfo.h"

/**
 * The Flower class ...
 */
class Flower {

public:
    Flower(float x, float y, MarkerPoint mp, float temp) :
        m_posX(x),
        m_posY(y),
        m_Reflectance(mp),
        m_fTemp(temp)
    {}

    //~Flower() {}

    ///
    float getTemp() const {return m_fTemp;}

    ///
    MarkerPoint getMarkerPoint() const {return m_Reflectance.getMarkerPoint();}

private:
    float m_posX;
    float m_posY;
    ReflectanceInfo m_Reflectance;
    float m_fTemp;
};

#endif /* _FLOWER_H */
