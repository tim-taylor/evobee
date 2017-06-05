/**
 * @file
 *
 * Declaration of the Pollinator class
 */

#ifndef _POLLINATOR_H
#define _POLLINATOR_H

#include "Position.h"
#include "AbstractHive.h"
#include "Environment.h"

class Environment;


/**
 * The Pollinator class ...
 */
class Pollinator {

public:
    Pollinator(AbstractHive* pHive);
    virtual ~Pollinator() {}

    /**
     * Move back to the start position, which is either the Hive position or
     * a random position, depending upon how the Hive is configured.
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
    fPos m_Position;
    AbstractHive* m_pHive;  // (non-owned) pointer to owning Hive
    Environment*  m_pEnv;   // (non-owned) pointer to Environment

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
};

#endif /* _POLLINATOR_H */
