/**
 * @file
 *
 * Implementation of the Patch class
 */

#include <iostream>
#include <sstream>
#include "Environment.h"
#include "Patch.h"
#include "FloweringPlant.h"
#include "tools.h"


Patch::Patch(Environment* pEnv, int posIdx, MarkerPoint mp, float temp) :
    m_pEnv(pEnv),
    m_fTemp(temp),
    m_posIdx(posIdx),
    m_BackgroundReflectance(mp),
    m_bReproConstraintsSetExplicitly(false),
    m_LocalityId(0),
    m_bSeedOutflowAllowed(true),
    m_bSeedOutflowRestricted(false),
    m_fSeedOutflowProb(1.0),
    m_bRefuge(false),
    m_iRefugeNativeSpecesId(0),
    m_fRefugeAlienInflowProb(1.0)
{
    m_Position = m_pEnv->getPatchCoordsFromIdx(posIdx);
}


// Add a plant to the patch based upon the given config, generating a brand new plant
// in the process
void Patch::addPlant(const PlantTypeConfig& typeConfig, const fPos& pos)
{
    /*
    if (ModelParams::verbose())
    {
        std::cout << "Adding plant of species " << typeConfig.species << " to Patch [" <<
            m_Position << "] at coordinates (" << pos << ")" << std::endl;
    }
    */

    m_FloweringPlants.emplace_back(typeConfig, pos, this);
}


// Add an existing plant to the patch.
// A move constructor is used, so the instance of the plant passed in as a parameter
// is wiped when the method is finished.
void Patch::addPlant(FloweringPlant& plant)
{
    m_FloweringPlants.push_back(std::move(plant));
}


void Patch::killAllPlants()
{
    m_FloweringPlants.clear();
}


bool Patch::inReproRestrictionArea(const iPos& dest) const
{
    bool inArea = true;
    if (m_bReproConstraintsSetExplicitly)
    {
        inArea = Environment::inArea(dest, m_ReproRestrictionAreaTopLeft, m_ReproRestrictionAreaBottomRight);
    }
    return inArea;
}


void Patch::setReproConstraints(const PlantTypeDistributionConfig& ptdc)
{
    if (m_bReproConstraintsSetExplicitly)
    {
        // If constraints have already been set on this patch and they
        // include refugia constraints, throw an exception as this is
        // an unresolvable situation (a patch can only be a refuge for
        // one species of plant).
        //
        // If, however, we are only dealing with constraints on seed outflow
        // (no refugia constraints) and these have already been set on this patch
        // patch, allow it if the new constraints are exactly the same as
        // the existing ones (otherwise throw an exception).
        //
        if ((ptdc.areaTopLeft == m_ReproRestrictionAreaTopLeft) &&
            (ptdc.areaBottomRight == m_ReproRestrictionAreaBottomRight) &&
            (ptdc.seedOutflowAllowed == m_bSeedOutflowAllowed) &&
            (ptdc.seedOutflowRestricted == m_bSeedOutflowRestricted) &&
            EvoBee::equal(ptdc.seedOutflowProb, m_fSeedOutflowProb) &&
            !(ptdc.refuge || m_bRefuge))
        {
            return;
        }
        else
        {
            std::stringstream msg;
            msg << "Attempt to set multiple sets of reproduction constraints on patch "
                << m_Position << ". Overlapping PlantTypeDistribution areas with "
                << "constraints on seed flow?";
            throw std::runtime_error(msg.str());
        }
    }

    m_LocalityId = ptdc.id;  // m_LocalityId is used to mark all patchs governed by the same ptdc
    m_ReproRestrictionAreaTopLeft = ptdc.areaTopLeft;
    m_ReproRestrictionAreaBottomRight = ptdc.areaBottomRight;
    m_bSeedOutflowAllowed = ptdc.seedOutflowAllowed;
    m_bSeedOutflowRestricted = ptdc.seedOutflowRestricted;
    m_fSeedOutflowProb = ptdc.seedOutflowProb;
    m_bRefuge = ptdc.refuge;
    m_iRefugeNativeSpecesId = FloweringPlant::getSpeciesId(ptdc.species);
    m_fRefugeAlienInflowProb = ptdc.refugeAlienInflowProb;

    m_bReproConstraintsSetExplicitly = true;
}