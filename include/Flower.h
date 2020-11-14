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
    /**
     * Constructor to create a brand new flower from config info at start of simulation
     */
    Flower(FloweringPlant* pPlant, const PlantTypeConfig& ptc, const fPos& pos, MarkerPoint mp);

    /**
     * Consructor to create an offspring flower based upon parent
     */
    Flower( FloweringPlant* pPlant, const Flower& parentFlower,
            const fPos& pos, const ReflectanceInfo& reflectance);

    // "Rule of 5" methods - https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
    Flower(const Flower& other);
    Flower(Flower&& other) noexcept;
    ~Flower() noexcept;
    Flower& operator= (const Flower& other);
    Flower& operator= (Flower&& other) noexcept;

    /**
     * Explicitly set the flower's owning plant
     * (only required when copying plants and flowers during reproduction)
     */
    void setPlant(FloweringPlant* plant) {m_pPlant = plant;}

    /**
     *
     */
    unsigned int getId() const {return m_id;}

    /**
     *
     */
    std::string getStateString() const;

    /**
     *
     */
    const fPos& getPosition() const {return m_Position;}

    /**
     *
     */
    float getTemperature() const {return m_fTemperature;}

    /**
     * Is the flower pollinated?
     */
    bool pollinated() const {return m_bPollinated;}

    /**
     *
     */
    unsigned int getSpeciesId() const {return m_SpeciesId;}

    /**
     * Return a string identifying the owning plant species
     */
    const std::string& getSpecies() const;

    /**
     *
     */
    //MarkerPoint getMarkerPoint() const {return m_Reflectance.getMarkerPoint();}

    /**
     *
     */
    Wavelength getCharacteristicWavelength() const {return m_Reflectance.getCharacteristicWavelength();}

    /*
     *
     */
    const ReflectanceInfo& getReflectanceInfo() const {return m_Reflectance;}

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
     * to the flower's stigma, if possible. Whether of not we allow pollen
     * from a different species to be deposited is determined by the flower's
     * m_bPollenClogging flag. If any of the deposited pollen is from the same
     * species, the flower will be pollinated. If we are unable to transfer
     * as much pollen from the pollinator as requested (because of limited
     * capacity of the stigma), then the excess is simply removed from the
     * pollinator as if lost in the air.
     *
     * @param pollinatorStore A reference to the pollinator's store from
     *  which the pollen is to be transferred
     * @param suggestedNum The number of grains that tha pollinator would normally
     *  transfer. The actual number transferred may be less than this if
     *  the stigma has reached its maximum capacity
     *
     * @return The number of grains transferred
     */
    int transferPollenFromPollinator(PollenVector& pollinatorStore, int suggestedNum);

    /**
     * Respond to a pollinator's request for nectar. Provide the full amount
     * requested if available, otherwise provide as much as is available. The
     * amount actually provided is indicated by the return value.
     */
    int collectNectar(int amountRequested);

    /**
     * Return a reference to the flower's vector of pollen on its stigma
     */
    const PollenVector& getStigmaPollen() const {return m_StigmaPollen;}

    /**
     *
     */
    bool pollenCloggingAll() const {return m_bPollenCloggingAll;}

    /**
     *
     */
     bool pollenCloggingPartial() const {return m_bPollenCloggingPartial;}


private:
    /**
     * Internal helper method for constructors and assignment operators
     */
    void copyCommon(const Flower& other) noexcept;


    unsigned int    m_id;               ///< Unique ID number for this flower
    unsigned int    m_SpeciesId;        ///< ID of the species (copied from the owning Plant's speciesID)
    fPos            m_Position;         ///< Spatial position of the flower
    ReflectanceInfo m_Reflectance;      ///< Flower reflectance info
    bool            m_bPollinated;      ///< Is the flower pollinated?
    int             m_iAntherPollen;    ///< Amount of collectable pollen remaining
    PollenVector    m_StigmaPollen;     ///< Collection of deposited Pollen grains on stigma
    int             m_iAvailableNectar; ///< Amount of nectar currently available for collection by pollinators
    float           m_fTemperature;     ///< Current temperature of flower
    FloweringPlant* m_pPlant;           ///< (non-owning) pointer to the plant this flower belongs to

    // the following are constant parameters for this flower
    int     m_iAntherPollenTransferPerVisit;  ///< Num pollen grains deposited on a pollinator per visit
    int     m_iStigmaMaxPollenCapacity;       ///< Maximum amount of pollen the stigma can carry
    bool    m_bPollenCloggingAll;
    bool    m_bPollenCloggingPartial;
    const std::vector<unsigned int>& m_CloggingSpeciesVec;

    /**
     * Record of next available unique ID number to be assigned to a new Flower
     */
    static unsigned int m_sNextFreeId;
};

#endif /* _FLOWER_H */
