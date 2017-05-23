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
    Flower();
    ~Flower() {}

    ///
    float getTemp() const {return m_fTemp;}

private:
    float m_fTemp;
    ReflectanceInfo m_Reflectance;
};

#endif /* _FLOWER_H */
