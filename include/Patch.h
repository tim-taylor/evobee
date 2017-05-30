/**
 * @file
 *
 * Declaration of the Patch class
 */

#ifndef _PATCH_H
#define _PATCH_H

#include <vector>
#include "ReflectanceInfo.h"
#include "PlantTypeConfig.h"
#include "FloweringPlant.h"
#include "Position.h"

class Environment;

using PlantVector = std::vector<FloweringPlant>;

/**
 * The Patch class ...
 */
class Patch {

public:
    Patch(Environment* pEnv, int posIdx, MarkerPoint backgroundMP, float temp);
    ~Patch() {}

    MarkerPoint getBackgroundMarkerPoint() const {return m_BackgroundReflectance.getMarkerPoint();}

    ///
    const iPos& getPosition() const {return m_Position;}

    ///
    void addPlant(const PlantTypeConfig& ptc, fPos pos);

    ///
    bool hasFloweringPlants() const {return !m_FloweringPlants.empty();}

    ///
    PlantVector& getFloweringPlants() {return m_FloweringPlants;}

private:
    Environment* m_pEnv;    ///< A pointer back to the owning Environment
    float m_fTemp;          ///< Ambient temperature of the patch
    int m_posIdx;           ///< The patch's unique index number in the Environment
    iPos m_Position;        ///< The patch's coordinates in Environment (derived from m_posIdx)
    ReflectanceInfo m_BackgroundReflectance;       ///< The patch's background reflectance properties
    PlantVector m_FloweringPlants; ///< All of the flowering plants on this patch
};

#endif /* _PATCH_H */
