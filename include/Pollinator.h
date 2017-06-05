/**
 * @file
 *
 * Declaration of the Pollinator class
 */

#ifndef _POLLINATOR_H
#define _POLLINATOR_H

#include "Position.h"
#include "AbstractHive.h"

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


protected:
    /**
     *
     */
    void moveRandom();
    void moveBiassed();
    void moveLevy();

    // protected data memebers
    AbstractHive* m_pHive;
    fPos m_Position;
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

};

#endif /* _POLLINATOR_H */
