/**
 * @file
 *
 * Declaration of the Patch class
 */

#ifndef _PATCH_H
#define _PATCH_H

#include "Colour.h"

class Environment;

/**
 * The Patch class ...
 */
class Patch {

public:
    Patch(Environment* pEnv, int posIdx, Colour& backgroundCol, float temp);
    ~Patch() {}

    ///
    void setColour(Colour& col) {m_Colour = col;}

    ///
    Colour& getColour() {return m_Colour;} 

    ///
    int getPosX() const {return m_posX;}

    ///
    int getPosY() const {return m_posY;}

private:
    Environment* m_pEnv;    ///< A pointer back to the owning Environment
    Colour m_Colour;        ///< The patch's background colour
    float m_fTemp;          ///< Ambient temperature of the patch
    int m_posIdx;           ///< The patch's unique index number in the Environment
    int m_posX;             ///< The patch's x coordinate in Environment (derived from m_posIdx)
    int m_posY;             ///< The patch's y coordinate in Environment (derived from m_posIdx)
};

#endif /* _PATCH_H */
