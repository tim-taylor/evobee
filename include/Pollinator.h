/**
 * @file
 *
 * Declaration of the Pollinator class
 */

#ifndef _POLLINATOR_H
#define _POLLINATOR_H

#include <string>
#include <vector>
#include "Position.h"
#include "AbstractHive.h"
#include "Environment.h"
#include "Flower.h"
#include "Pollen.h"
#include "PollinatorConfig.h"

class Environment;


/**
 * The PollinatorState enum
 */
enum class PollinatorState {UNINITIATED, FORAGING, BOUT_COMPLETE};


/**
 * The Pollinator class ...
 */
class Pollinator {

public:
    Pollinator(const PollinatorConfig& pc, AbstractHive* pHive);

    // "Rule of 5" methods - https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
    Pollinator(const Pollinator& other);
    Pollinator(Pollinator&& other) noexcept;
    virtual ~Pollinator() noexcept;
    Pollinator& operator= (const Pollinator& other);
    Pollinator& operator= (Pollinator&& other) noexcept;    

    /**
     * Peform a complete reset of this pollinator to its initial state,
     * resetting position and all other state variables as if this
     * was a brand new pollinator
     */
    virtual void reset();

    /**
     * Move back to the start position, which is either the Hive position or
     * a random position, depending upon how the Hive is configured. Also
     * reset to a random heading.
     */
    void resetToStartPosition();

    /**
     *
     */
    const fPos& getPosition() const {return m_Position;}

    /**
     *
     */
    virtual void step() = 0;

    /**
     *
     */
    Environment* getEnvironment() {return m_pHive->getEnvironment();}

    /**
     * Query whether this pollinator is currently within the bounds of the Environment
     */
    bool inEnvironment() const {return m_pEnv->inEnvironment(m_Position);}

    /**
     * Query whether this pollinator is currently within its allowed area,
     * taking into account whether migration is allowed to outside of the
     * Hive's initial area
     */
    bool inAllowedArea() const;

    /**
     * Returns a string representation of the pollinator's current state, suitable
     * for writing to a log file
     */
    virtual std::string getStateString() const;

    /**
     *
     */
    virtual const std::string& getTypeName() const;

    /**
     *
     */
    unsigned int getId() const {return m_id;}

    /**
     *
     */
    static unsigned int getMaxIdIssued() {return m_sNextFreeId-1;}

    /**
     *
     */
    PollinatorState getState() const {return m_State;}


protected:
    /**
     * Move by the given distance in a uniform random direction
     * @param allowOffEnv Do we allow move to take pollinator out of the bounds of the environment?
     * @return true if pollinator is within the bounds of the environment after the move, false otherwise
     */
    void moveRandom(/*bool allowOffEnv = false,*/ float stepLength = 1.0);

    /**
     * Move by the given distance in a direction biassed by the experimental 
     * bee flight data recorded by Waddington
     */    
    bool moveBiassed(bool allowOffEnv = false, float stepLength = 1.0);

    /**
     * Move according to a Levy flight pattern: direction of travel is 
     * uniform random, and distance travelled is selected according to a
     * specific probability distribution (TBC)
     */    
    bool moveLevy(bool allowOffEnv = false, float stepLength = 1.0);

    /**
     * Increment the landing count of each Pollen grain in the store. 
     * This method should be called each time a Pollinator lands on a flower.
     * Note that this method does not remove any Pollen from the store; to 
     * remove grains that now exceed their carryover limit, call 
     * removeOldCarryoverPollen() afterwards.
     */
    void updatePollenLandingCount();

    /**
     * Remove any stored pollen that has now exceeded the maximum number of
     * flower visits subsequent to being collected, as defined by the
     * carryover limit.
     */
    void removeOldCarryoverPollen();

    /**
     * Transfer some of our pollen to the flower (potentially pollinating it)
     *
     * @return The number of pollen grains deposited
     */
    int depositPollenOnStigma(Flower* pFlower);

    /**
     * Collect pollen from flower if available, and add to our store.
     * If our store exceeds its maximum capacity after collecting new pollen,
     * a random selection of pollen grains are deleted from the store to 
     * bring it back down to its capacity.
     */
    void collectPollenFromAnther(Flower* pFlower);

    /**
     * Lose the specified amount of pollen to the air.
     *
     * @return The number of pollen grains lost (which might be less than the
     *  number requested if the store originally contained less than that amount)
     */
    int losePollenToAir(int num);


    // protected data memebers
    unsigned int    m_id;       ///< Unique ID number for this pollinator
    fPos            m_Position; ///< Pollinator's current position in environment
    float           m_fHeading; ///< Pollinator's current heading (between 0.0 - TWOPI)
    AbstractHive*   m_pHive;    ///< (non-owned) pointer to owning Hive
    Environment*    m_pEnv;     ///< (non-owned) pointer to Environment
    const EvoBeeModel* m_pModel;///< (non-owned) pointer to EvoBeeModel

    PollinatorState m_State;    ///< The current state of the pollinator
    int             m_iNumFlowersVisitedInBout; ///< Number of flowers visited so far in current bout
    PollenVector    m_PollenStore;              ///< Container for Pollen currently being carried

    // some constant parameters for this pollinator
    const int       m_iBoutLength;          ///< Number of flower visits pollinator can make before returning to hive
    const int       m_iPollenLossOnFlower;  ///< Amount of pollen deposited on a flower on each visit
    const int       m_iPollenLossInAir;     ///< Amount of pollen lost on each timestep when flying
    const int       m_iMaxPollenCapacity;   ///< Maximum amount of pollen the pollinator can carry
    const int       m_iPollenCarryoverNumVisits; /** After collecting a grain of pollen from a flower,
                                                  * the pollinator can visit this number of subsequent
                                                  * flowers to potentially deposit it. If the pollen is
                                                  * still not deposited after this numebr of visits, it
                                                  * is is removed from the pollinator (i.e. it is lost)
                                                  */

    /*
    position (float)
    controller (commute speed, steering tendancy [random, biased, levy], )
    load capacity
    flower handling time
    [mass, shape]
    visual acuity spec
    colour recog spec
    */

private:
    /*
     * Helper method to reposition the pollinator within the environment
     * after it has attempted to move beyond its limits
     */
    void repositionInEnv(fPos delta);

    /*
     * Helper method to reposition the pollinator within the allowed area
     * specifed by the Hive after it has attempted to move beyond its limits
     */
    void repositionInAllowedArea(fPos delta); 

    /*
     * Helper method to reposition the pollinator within the specified area
     * after it has attempted to move beyond its limits
     */
    void repositionInArea(fPos delta, float minx, float miny, float maxx, float maxy);    

    /**
     * Record of next available unique ID number to be assigned to a new Pollinator
     */
    static unsigned int m_sNextFreeId;

    /**
     * String description of the Pollinator type, for use in log files
     */    
    static std::string m_sTypeNameStr;
};

#endif /* _POLLINATOR_H */
