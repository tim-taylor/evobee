/**
 * @file
 *
 * Declaration of the Pollinator class
 */

#ifndef _POLLINATOR_H
#define _POLLINATOR_H

#include <string>
#include <random>
#include "Position.h"
#include "AbstractHive.h"
#include "Environment.h"
#include "PollinatorConfig.h"

class Environment;


/**
 * The Pollinator class ...
 */
class Pollinator {

public:
    Pollinator(const PollinatorConfig& pc, AbstractHive* pHive);
    virtual ~Pollinator() {}

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
     * Returns a string representation of the pollinator's current state, suitable
     * for writing to a log file
     */
    virtual std::string getStateString() const;

    /**
     *
     */
    virtual const std::string& getTypeName() const;


protected:
    /**
     * Move by the given distance in a uniform random direction
     * @param allowOffEnv Do we allow move to take pollinator out of the bounds of the environment?
     * @return true if pollinator is within the bounds of the environment after the move, false otherwise
     */
    bool moveRandom(bool allowOffEnv = false, float stepLength = 1.0);

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

    // protected data memebers
    unsigned int    m_id;       ///< Unique ID number for this pollinator
    fPos            m_Position; ///< Pollinator's current position in environment
    float           m_fHeading; ///< Pollinator's current heading (between 0.0 - TWOPI)
    AbstractHive*   m_pHive;    ///< (non-owned) pointer to owning Hive
    Environment*    m_pEnv;     ///< (non-owned) pointer to Environment

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

    int             m_iPollenLoad;              ///< The amount of pollen currently being carried
    int             m_iNumFlowersVisitedInBout; ///< Number of flowers visited so far in current bout


    static std::uniform_real_distribution<float> m_sDirectionDistrib; ///< Uniform distribution between 0.0 to TWOPI

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
