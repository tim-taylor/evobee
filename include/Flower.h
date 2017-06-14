/**
 * @file
 *
 * Declaration of the Flower class
 */

#ifndef _FLOWER_H
#define _FLOWER_H

#include <vector>
#include "ReflectanceInfo.h"
#include "Position.h"
#include "PlantTypeConfig.h"
#include "Pollen.h"

using PollenVector = std::vector<Pollen>;

class FloweringPlant;


/**
 * The Flower class ...
 */
class Flower {

public:
    Flower(FloweringPlant* pPlant, const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp);

    ///
    float getTemp() const {return m_fTemp;}

    /**
     * Is the flower pollinated?
     */
    bool pollinated() const {return m_bPollinated;}

    /**
     * Return a string identifying the owning plant species
     */
    const std::string& getSpecies() const;

    ///
    MarkerPoint getMarkerPoint() const {return m_Reflectance.getMarkerPoint();}

    /**
     * Transfer some pollen grains from the anthers, if available, to the
     * specified pollinator's store.
     *
     * @param pollinatorStore A reference to the pollinator's store into
     *  which the pollen is to be transferred
     *
     * @return The number of grains transferred
     *
     * Note, no check is made as to whether the capacity of the pollinator's
     * store has been exceeded. It is the responsibility of the pollinator to
     * perform such a check after it has called this method.
     */
    int transferAntherPollenToPollinator(PollenVector& pollinatorStore);

    /**
     * Transfer some pollen grains from the specified pollinator's store
     * to the flower's stigma, if possible. If any of the deposited pollen
     * is from the same species, the flower will be pollinated.
     *
     * @param pollinatorStore A reference to the pollinator's store from
     *  which the pollen is to be transferred
     * @param suggestedNum The number of grains that tha pollinator would normally
     *  transfer. The actual number transferred may be less than this if
     *  the stigma has reached its maximum capacity
     * @param sameSpeciesOnly If true, only pollen from the same species as
     *  this flower will be transferred. If false, all pollen will be 
     *  considered for transfer (so pollen clogging may occur)
     *
     * @return The number of grains transferred
     */
    int transferPollenFromPollinator(PollenVector& pollinatorStore, int suggestedNum, bool sameSpeciesOnly);

private:
    fPos            m_Position;         ///< Spatial position of the flower
    ReflectanceInfo m_Reflectance;      ///< Flower reflectance info
    bool            m_bPollinated;      ///< Is the flower pollinated?
    int             m_iAntherPollen;    ///< Amount of collectable pollen remaining
    PollenVector    m_StigmaPollen;     ///< Collection of deposited Pollen grains on stigma
    float           m_fTemp;            ///< Current temperature of flower

    FloweringPlant* m_pPlant;           ///< (non-owning) pointer to the plant this flower belongs to

    // the following are constant parameters for this flower
    const int   m_iAntherPollenLossPerVisit;///< Number of pollen grains deposited on a pollinator per visit
    const int   m_iStigmaMaxPollenCapacity; ///< Maximum amount of pollen the stigma can carry
    /*const int m_iNectarRewardPerVisit;    ///< Amount of reward given to a pollinator (CURRENTLY UNUSED)*/
};

#endif /* _FLOWER_H */
