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

    /**
     *
     */
    Environment* getEnvironment() {return m_pEnv;}

    /**
     *
     */
    bool migrationAllowed() const {return m_bMigrationAllowed;}
    
    /**
     *
     */
    bool migrationRestricted() const {return m_bMigrationRestricted;}

    /**
     *
     */
    float migrationProb() const {return m_fMigrationProb;}    

    /**
     * Return the coordinate of the top left corner of the initial foraging area
     * for pollinators from this hive, as defined by the Hive->area-top-left-x/y
     * params in the configuration file. Note that this area is *only* used to
     * restrict the initial placement of pollinators. Once a foraging phase
     * commences, pollinator movement restrictions are determined by the
     * pollinator's m_MovementAreaTL/BR variables. These are initialised
     * from the local *Patch*'s m_ReproRestrictionAreaTL/BR which is defined
     * by the PlantTypeDistribution->area-top-left-x/y params in the config
     * file. The pollinator's m_MovementAreaTL/BR may be updated during a
     * foraging phase if the pollinator successfully migrates to a new
     * area (this happens in Pollinator::inAllowedArea(), called from the
     * Pollinator moveXXX methods).
     */
    const iPos& getInitForageAreaTopLeft() const {return m_InitForageAreaTopLeft;}

    /**
     * See comments for AbstractHive::getInitForageAreaTopLeft() for more
     * information about this method.
     */    
    const iPos& getInitForageAreaBottomRight() const {return m_InitForageAreaBottomRight;}

private:
    Environment* m_pEnv;                ///< A pointer to the Environment in which the Hive is placed
    fPos  m_Position;                   ///< Hive position in environment
    bool  m_bStartFromHive;             ///< Do Pollinators start from hive, or at a random position?
    iPos  m_InitForageAreaTopLeft;      ///< Top-left corner of initial foraging area
                                        ///<  (see AbstractHive::getInitForageAreaTopLeft() for
                                        ///<   details of usage of this variable)
    iPos  m_InitForageAreaBottomRight;  ///< Bottom-right corner of initial foraging area
                                        ///<  (see AbstractHive::getInitForageAreaTopLeft() for
                                        ///<   details of usage of this variable)
    bool  m_bMigrationAllowed;          ///< Can pollinators migrate out of initial foraging area?
    bool  m_bMigrationRestricted;       ///< If m_bMigrationAllowed, is prob of doing so restricted?
    float m_fMigrationProb;             ///< If migration allowed restricted, and pollinator tries to
                                        ///< leave init foraging area, it succeeds with this prob,
                                        ///< else it is reflected back into init area
};

#endif /* _ABSTRACTHIVE_H */
