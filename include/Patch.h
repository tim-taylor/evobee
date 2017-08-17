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
    //~Patch() {}

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
    void addPlant(const PlantTypeConfig& typeConfig, const fPos& pos);


    /**
     * Add an existing plant to the patch.
     * A move constructor is used, so the instance of the plant passed in as a parameter
     * is wiped when the method is finished. This is used during the reproduction procedure.
     */
    void addPlant(FloweringPlant& plant);

    /**
     * Delete all plants from this patch
     */
    void killAllPlants();

    /**
     *
     */
    bool hasFloweringPlants() const {return !m_FloweringPlants.empty();}

    /**
     *
     */
    PlantVector& getFloweringPlants() {return m_FloweringPlants;}

    /**
     *
     */
    const PlantVector& getFloweringPlants() const {return m_FloweringPlants;}

    /**
     * Explicitly set the constraints on plant reproduction and seed flow for
     * this patch. This can only be done once for each patch! If the system is
     * trying to do it multiple times it is probably because there are overlapping
     * PlantTypeDistributions areas which are either refuges and/or have seedFlow
     * restrictions. These types of areas are not allowed to overlap.
     */
    void setReproConstraints(const PlantTypeDistributionConfig& pdcfg);

    /**
     * Return the id of the locality to which this patch belongs.
     * The locality id of a patch is equal to the id of the PlentTypeDistributionConfig
     * that sets constraints on seed flow etc for this patch (see Patch::setReproConstraints,
     * which is called by Environment::initialisePlants at the start of the run).
     */
    unsigned int getLocalityId() const {return m_LocalityId;}

    /**
     * Is the specified destination patch within the same restricted reproduction
     * area as this patch?
     * If this patch has no reproduction restrictions (i.e., if
     * m_bReproConstraintsSetExplicitly == false), then this method will always
     * return true.
     */
    bool inReproRestrictionArea(const iPos& dest) const;

    /**
     * During reproduction, is seed flow allowed from this patch to to a destination
     * patch outside of this patch's restricted reproduction area?
     * NB: you should first call inReproRestrictionArea(dest) to check whether
     * this restruction applies at all to the destination patch under consideration
     * (only applies if inReproRestrictionArea(dest) == false)
     */
    bool seedOutflowAllowed() const {return m_bSeedOutflowAllowed;}

    /**
     * If seedOutflowAllowed() is true, is the probability of successful seed
     * flow restricted at all (i.e. probability less than 1)?
     * NB: you should first call inReproRestrictionArea(dest) to check whether
     * this restruction applies at all to the destination patch under consideration
     * (only applies if inReproRestrictionArea(dest) == false)
     */
    bool seedOutflowRestricted() const {return m_bSeedOutflowRestricted;}

    /**
     * If seedOutflowAllowed() and seedOutflowRestricted() are both true,
     * what is the probability of successful seed flow to the destination
     * patch?
     * NB: you should first call inReproRestrictionArea(dest) to check whether
     * this restruction applies at all to the destination patch under consideration
     * (only applies if inReproRestrictionArea(dest) == false)
     */
    float getSeedOutflowProb() const {return m_fSeedOutflowProb;}

    /**
     *
     */
    bool refuge() const {return m_bRefuge;}

    /**
     *
     */
    unsigned int getRefugeNativeSpeciesId() const {return m_iRefugeNativeSpecesId;}

    /**
     *
     */
    float getRefugeAlienInflowProb() const {return m_fRefugeAlienInflowProb;}

    /**
     *
     */
    const iPos& getReproRestrictionAreaTopLeft() const {return m_ReproRestrictionAreaTopLeft;}

    /**
     *
     */
    const iPos& getReproRestrictionAreaBottomRight() const {return m_ReproRestrictionAreaBottomRight;}


private:
    Environment*    m_pEnv;         ///< A pointer back to the owning Environment
    float           m_fTemp;        ///< Ambient temperature of the patch
    int             m_posIdx;       ///< The patch's unique index number in the Environment
    iPos            m_Position;     ///< The patch's coordinates in Environment (derived from m_posIdx)
    ReflectanceInfo m_BackgroundReflectance; ///< The patch's background reflectance properties
    PlantVector     m_FloweringPlants;       ///< All of the flowering plants on this patch

    // The following parameters place restrictions on plant reproduction
    bool            m_bReproConstraintsSetExplicitly;
    unsigned int    m_LocalityId;   ///< if repro constraints set, this is set to the PlantTypeDistConfig
                                    ///<   responsible for definining the constraints. Therefore, all patches
                                    ///<   governed by the same PlantTypeDistributionConfig have the same
                                    ///<   value for locality id. This is used so that we can easily determine
                                    ///<   for any patch what constraints exist, if any, on seed flow etc.
    iPos            m_ReproRestrictionAreaTopLeft;
    iPos            m_ReproRestrictionAreaBottomRight;
    bool            m_bSeedOutflowAllowed;
    bool            m_bSeedOutflowRestricted;
    float           m_fSeedOutflowProb;
    bool            m_bRefuge;
    unsigned int    m_iRefugeNativeSpecesId;
    float           m_fRefugeAlienInflowProb;
};

#endif /* _PATCH_H */
