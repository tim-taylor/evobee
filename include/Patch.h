/**
 * @file
 *
 * Declaration of the Patch class
 */

#ifndef _PATCH_H
#define _PATCH_H

#include <vector>
//#include "Colour.h"
#include "ReflectanceInfo.h"
#include "PlantTypeConfig.h"
#include "FloweringPlant.h"

class Environment;

/**
 * The Patch class ...
 */
class Patch {

public:
    //Patch(Environment* pEnv, int posIdx, const Colour& backgroundCol, float temp);
    //Patch(Environment* pEnv, int posIdx, const ReflectanceInfo& reflectance, float temp);
    Patch(Environment* pEnv, int posIdx, MarkerPoint backgroundMP, float temp);
    ~Patch() {}

    ///
    //void setColour(Colour& col) {m_Colour = col;}

    ///
    //Colour& getColour() {return m_Colour;} 

    MarkerPoint getBackgroundMarkerPoint() const {return m_BackgroundReflectance.getMarkerPoint();}

    ///
    int getPosX() const {return m_posX;}

    ///
    int getPosY() const {return m_posY;}

    ///
    void addPlant(const PlantTypeConfig& ptc, float x, float y);

    ///
    bool hasPlants() const {return !m_FloweringPlants.empty();}

private:
    Environment* m_pEnv;    ///< A pointer back to the owning Environment
    ReflectanceInfo m_BackgroundReflectance; ///< The patch's background reflectance properties
    //Colour m_Colour;        ///< The patch's background colour
    float m_fTemp;          ///< Ambient temperature of the patch
    int m_posIdx;           ///< The patch's unique index number in the Environment
    int m_posX;             ///< The patch's x coordinate in Environment (derived from m_posIdx)
    int m_posY;             ///< The patch's y coordinate in Environment (derived from m_posIdx)
    std::vector<FloweringPlant> m_FloweringPlants; ///< All of the flowering plants on this patch
};

#endif /* _PATCH_H */
