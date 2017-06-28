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
#include "PlantTypeDistributionConfig.h"
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

    /**
     *
     */
    MarkerPoint getBackgroundMarkerPoint() const {return m_BackgroundReflectance.getMarkerPoint();}

    /**
     *
     */
    const iPos& getPosition() const {return m_Position;}

    /**
     * Create a new plant object based upon the specified config objects and add
     * it to this patch at the specified position
     */
    void addPlant(const PlantTypeDistributionConfig& distConfig,
                  const PlantTypeConfig& typeConfig, fPos pos);

    /**
     *
     */    
    bool hasFloweringPlants() const {return !m_FloweringPlants.empty();}

    /**
     *
     */    
    PlantVector& getFloweringPlants() {return m_FloweringPlants;}

    /**
     * Explicitly set the constraints on plant reproduction and seed flow for
     * this patch. This can only be done once for each patch! If the system is
     * trying to do it multiple times it is probably because there are overlapping
     * PlantTypeDistributions areas which are either refuges and/or have seedFlow
     * restrictions. These types of areas are not allowed to overlap.
     */
    void setReproConstraints(const PlantTypeDistributionConfig& pdcfg);


private:
    Environment*    m_pEnv;         ///< A pointer back to the owning Environment
    float           m_fTemp;        ///< Ambient temperature of the patch
    int             m_posIdx;       ///< The patch's unique index number in the Environment
    iPos            m_Position;     ///< The patch's coordinates in Environment (derived from m_posIdx)
    ReflectanceInfo m_BackgroundReflectance; ///< The patch's background reflectance properties
    PlantVector     m_FloweringPlants;       ///< All of the flowering plants on this patch

    // The following parameters place restrictions on plant reproduction
    bool            m_bReproConstraintsSetExplicitly;
    bool            m_bSeedOutflowAllowed;     
    bool            m_bSeedOutflowRestricted;
    float           m_bSeedOutflowProb;
    bool            m_bRefuge;
    unsigned int    m_iRefugeNativeSpecesId;
    float           m_fRefugeAlienInflowProb;
};

#endif /* _PATCH_H */
