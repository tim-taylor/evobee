/**
 * @file
 *
 * Declaration of the AbstractHive class
 */

#ifndef _ABSTRACTHIVE_H
#define _ABSTRACTHIVE_H

#include <memory>
#include "HiveConfig.h"
#include "Position.h"

class Environment;
class Pollinator;


/**
 * The AbstractHive class provides a generic interface and members
 * for all Hives of a specific type of Pollinator. Specific hives
 * are instances of the Hive class, which is templated for the
 * specific Pollinator. The Hive classes are derived from AbstractHive.
 */
class AbstractHive {

public:
    AbstractHive(Environment* pEnv, const HiveConfig& hc);
    virtual ~AbstractHive() {}

    /**
     * A static factory method to produce a Hive of the subtype specified
     * by hc.type, and return a shared pointer to the created object
     */
    static std::shared_ptr<AbstractHive> makeHive(Environment* pEnv, const HiveConfig& hc);

    /**
     *
     */
    const fPos& getPosition() const {return m_Position;}

    /**
     *
     */
    fPos getRandomPollinatorStartPosition() const;

    /**
     *
     */
    bool startFromHive() const {return m_bStartFromHive;}

    /**
     *
     */
    virtual int getNumPollinators() const = 0;
    
    /**
     *
     */
    virtual Pollinator* getPollinator(size_t idx) = 0;

private:
    Environment* m_pEnv;    ///< A pointer to the Environment in which the Hive is placed
    fPos  m_Position;       ///< Hive position in environment
    bool  m_bStartFromHive; ///< Do Pollinators start from hive, or at a random position?
};

#endif /* _ABSTRACTHIVE_H */
