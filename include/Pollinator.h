/**
 * @file
 *
 * Declaration of the Pollinator class and helper enums and structs
 * PollinatorState and PollinatorPerformanceInfo
 */

#ifndef _POLLINATOR_H
#define _POLLINATOR_H

#include <string>
#include <vector>
#include <map>
#include "Position.h"
#include "AbstractHive.h"
#include "Environment.h"
#include "Flower.h"
#include "Pollen.h"
#include "PollinatorConfig.h"
#include "PollinatorEnums.h"
#include "PollinatorStructs.h"

class Environment;

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
     * Default implemenation of the pollinator's update logic at each step of the
     * foraging phase. May be overridden by subclasses.
     */
    virtual void step();

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
     * Hive's initial area.
     *
     * NB This method also considers the migration restrictions defined for the
     * Hive. If the pollinator has moved out of its previously allowed movement
     * area and the migration is deemed to be allowed, then the pollinator's
     * allowed movement area is updated according to the reproRestructionArea of
     * the new patch. Hence, this is NOT a const method.
     *
     * Also see the comments for the AbstractHive::getInitForageAreaTopLeft() method
     * for more information about the relationship between
     * Pollinator::m_MovementAreaTL/BR and AbstractHive::m_InitForageAreaTL/BR.
     */
    bool inAllowedArea();

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

    /**
     * Returns the number of pollen grains of the specified plant species
     * currently being carried by the pollinator
     */
    int getNumPollenGrainsInStore(unsigned int speciesId) const;

    /**
     *
     */
    const std::map<unsigned int, PollinatorPerformanceInfo>& getPerformanceInfoMap() const {return m_PerformanceInfoMap;}

    /**
     * Checks whether the pollinator classifies the given visual stimulus as a match against its
     * current target marker point.
     */
    // This is now obsolete - use confidenceMatchesTarget() instead
    //bool matchesTargetMP(const ReflectanceInfo& stimulus) const;

    /**
     * Compares a given stimulus to the pollinator's current target marker point and
     * returns a confidence level between 0.0 and 1.0 that the stimulus matches
     * the target.
     */
    float confidenceMatchesTarget(const ReflectanceInfo& stimulus) const;

    /**
     * Returns the pollinator's current target marker point.
     * N.B. If the pollinator does not yet have a target, the returned value will be 0 (=NO_MARKER_POINT)
     */
    Wavelength getTargetWavelength() const {return m_TargetReflectance.getCharacteristicWavelength();}

    /*
     * Set the pollinator's target marker point.
     * This is only expected to be called by the pollinator's code itself or by external testing code. It
     * should not be called by external code under normal circumstances.
     */
    void setTargetWavelength(Wavelength lambda);
    void setTargetWavelength(const VisualStimulusInfo* pVSI);

    /**
     * Default implementation of a method to make a probabilistic decision,
     * for a given characteristic wavelength, on whether the pollinator can
     * detect it. May be overriden by subclasses.
     */
    virtual bool isDetected(const ReflectanceInfo& rinfo) const;


protected:

    /**
     * Default implementation of Random foraging strategy.
     * May be overridden by subclasses.
     */
    virtual void forageRandom();

    /**
     * Default implementation of Random foraging strategy.
     * May be overridden by subclasses.
     */
    virtual void forageRandomGlobal();

    /**
     * Default implementation of Nearest Flower foraging strategy.
     * May be overridden by subclasses.
     */
    virtual void forageNearestFlower();

    /**
     * Default implementation of Random Flower foraging strategy.
     * May be overridden by subclasses.
     */
    virtual void forageRandomFlower();

    /**
     * Default implementation of method to determine whether the pollinator should
     * harvest the specified flower. May be overridden by subclasses.
     */
    virtual bool isVisitCandidate(Flower* pFlower, bool* pJudgedToMatchTarget = nullptr) const;

    /**
     * Default implementation of method to determine whether the pollinator should
     * harvest the specified flower using its visual perception. This is a special case
     * called by isVisitCandidate() in the case that m_ConstancyType is VISUAL.
     * May be overridden by subclasses.
     */
    virtual bool isVisitCandidateVisual(Flower* pFlower, bool* pJudgedToMatchTarget = nullptr) const;

    /**
     * Default implementation of method to update the pollinator's visual preference
     * data after each visit to a flower. May be overridden by subclasses.
     */
    virtual void updateVisualPreferences(const Flower* pFlower, int nectarCollected);

    /**
     * Default implementation of method coding the logic of what happens when the
     * pollinator visits a flower. May be overridden by subclasses.
     *
     * @return the amount of nectar collected on this visit
     */
    virtual int visitFlower(Flower* pFlower);

    /**
     * Move in a random direction by a distance determined by m_fStepLength.
     */
    void moveRandom();

    /**
     * Move according to a Levy flight pattern: direction of travel is
     * uniform random, and distance travelled is selected according to a
     * Levy probability distribution. The scale parameter of the Levy
     * probability density function is set by Pollinator::m_fStepLength.
     */
    void moveLevy();

    /**
     * Reset this pollinator's allowed movement area according to the
     * ReproRestrictionArea constraints of the current Patch
     */
    void resetMovementArea();

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
     * Also, if we are in a logging mode that cares about it, update the
     * pollinator's performance info about number of landings and pollinations
     * of the specified flower species.
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
     * Collect nectar reward from flower if available.
     */
    int collectNectar(Flower* pFlower);

    /**
     * Lose the specified amount of pollen to the air.
     *
     * @return The number of pollen grains lost (which might be less than the
     *  number requested if the store originally contained less than that amount)
     */
    int losePollenToAir(int num);

    /**
     * Returns the probability that this pollinator detects a visual stimulus which
     * possesses a single marker point as specified by the mp parameter.
     *
     * This is a pure virtual method that should be overridden by derived classes.
     *
     * @return Detection probablility in the range 0.0 to 1.0
     */
    virtual float getMPDetectionProb(MarkerPoint mp) const = 0;

    /**
     * Returns true if a flower with the specified marker point is in the list of recently visited flowers
     */
    //bool isRecentlyVisited(MarkerPoint mp) const;

    /**
     * Returns information about how a given stimulus with a single marker point
     * as specified by the mp parameter is perceived by this pollinator's visual
     * system.
     *
     * This is a pure virtual method that should be overridden by derived classes.
     *
     * @return A const reference to a VisualStimulusInfo which specifies the stimulus's
     * location in hexagonal colour space, its green contrast, and detection probability
     */
    virtual const VisualStimulusInfo& getVisStimulusInfo(MarkerPoint mp) const = 0;

    /**
     * Calculate the Euclidean distance between two points in hexagonal colour space
     * If the optional parameter usePureSpectralPoints is set to true, the distance is calculated
     * from the points on the pure spectral line of the two stimuli instead of their raw positions
     * in hex space.
     */
    static float getVisHexDistance(const VisualStimulusInfo& infoStimulus, const VisualStimulusInfo& infoTarget,
                                   bool usePureSpectralPoints = false);


    // protected data members
    unsigned int    m_id;       ///< Unique ID number for this pollinator
    fPos            m_Position; ///< Pollinator's current position in environment
    float           m_fHeading; ///< Pollinator's current heading (between 0.0 - TWOPI)
    AbstractHive*   m_pHive;    ///< (non-owned) pointer to owning Hive
    Environment*    m_pEnv;     ///< (non-owned) pointer to Environment
    const EvoBeeModel* m_pModel;///< (non-owned) pointer to EvoBeeModel

    PollinatorState m_State;    ///< The current state of the pollinator

    PollinatorLatestAction m_LatestAction;      ///< Record of the latest action made by the pollinator,
                                                ///<   updated at each step

    int             m_iNumFlowersVisitedInBout; ///< Number of flowers visited so far in current bout

    int             m_iCollectedNectar;         ///< Total amount of nectar currently collected from flowers

    PollenVector    m_PollenStore;              ///< Container for Pollen currently being carried

    iPos            m_MovementAreaTopLeft;      ///< Boundary of area in which pollinator is allowed to move
                                                ///<  (see AbstractHive::getInitForageAreaTopLeft() for
                                                ///<   details of usage of this variable)
    iPos            m_MovementAreaBottomRight;  ///< Boundary of area in which pollinator is allowed to move
                                                ///<  (see AbstractHive::getInitForageAreaTopLeft() for
                                                ///<   details of usage of this variable)

    //MarkerPoint     m_TargetMP;               ///< This pollinator's current target flower colour
                                                ///<   marker point
                                                ///< THIS IS OBSOLETE - now replaced by m_TargetReflectance

    ReflectanceInfo m_TargetReflectance;        ///< This pollinator's current target flower colour information

    PollinatorConstancyType m_ConstancyType;    ///< Specification of what type of constancy (if any)
                                                ///<   this pollinator exhibits in flower visits
    float           m_fConstancyParam;          ///< Parameter for constancy (interpretation depends
                                                ///<   on value of m_ConstancyType)
    unsigned int    m_PreviousLandingSpeciesId; ///< Species id of the most recently visited
                                                ///<   flower by this pollinator (or 0 if none visited)

    unsigned int    m_uiVisitedFlowerMemorySize;///< The maximum number of recently visited flowers that
                                                ///<   the pollinator can remember

    std::vector<Flower*> m_RecentlyVisitedFlowers;  ///< A record of the ids for recently visited
                                                    ///<   flowers (up to maximum length defined
                                                    ///<   by m_uiVisitedFlowerMemorySize)
                                                    ///<   NB: this vector records individual Flower ids,
                                                    ///<   in contrast to m_PreviousLandingSpeciesId
                                                    ///<   which records the previous *speciesId*

    PollinatorForagingStrategy m_ForagingStrategy;  ///< Determines exactly how the pollinator moves at each step

    PollinatorLearningStrategy m_LearningStrategy;  ///< Determines how the pollinator updates its preferences
                                                    ///<    for landing on different types of flower in light
                                                    ///<    on its experience of rewards gained

    int             m_iPresetPrefVisDataID;         ///< id number to identify the preset innate preference,
                                                    ///<    set when param innate-preference-type=preset
                                                    ///<    The id number is a key in the Hymenoptera::m_sVisData vector

    // some constant parameters for this pollinator
    const int       m_iBoutLength;                  ///< Num flower visits allowed before returning to hive [0=unlimited]
    const PollinatorStepType m_StepType;            ///< The type of steps the pollinator makes while foraging (e.g. constant, levy)
    const float     m_fStepLength;                  ///< Parameter determining size of individual hops while foraging
    const int       m_iPollenDepositPerFlowerVisit; ///< Amount of pollen deposited on a flower on each visit
    const int       m_iPollenLossInAir;             ///< Amount of pollen lost on each timestep when flying
    const int       m_iMaxPollenCapacity;           ///< Maximum amount of pollen the pollinator can carry
    const int       m_iPollenCarryoverNumVisits;    /** After collecting a grain of pollen from a flower,
                                                    * the pollinator can visit this number of subsequent
                                                    * flowers to potentially deposit it. If the pollen is
                                                    * still not deposited after this number of visits, it
                                                    * is removed from the pollinator (i.e. it is lost)
                                                    * A value of 0 indicates no limit in number of visits.
                                                    */
    const int       m_iNectarCollectPerFlowerVisit; ///< Amount of nectar pollinator attempts to collect
                                                    ///< from a flower on a single visit

    const PollinatorInnatePrefType m_InnatePreferenceType; ///< The pollinator's innate preference type (e.g. giurfa, flat, hoverfly)

    // and some other parameters that have some dependence on the constant parameters above

    const VisualStimulusInfo* m_PresetPrefVisDataPtr; ///< In postprocessing after reading in the config file, m_iPresetPrefVisDataID
                                                    ///<   is used to find a pointer to the identified entry in vis-data, which is
                                                    ///<   recorded here (only used when innate-preference-type=preset)

    /*
     * Some other properties that might be implemented in future versions include:
     * - controller (commute speed, steering tendancy [random, biased, levy], )
     * - flower handling time
     * - [mass, shape]
     * - visual acuity spec
     */

     // housekeeping variables to keeping track of pollinator's performance during a foraging phase
     std::map<unsigned int, PollinatorPerformanceInfo> m_PerformanceInfoMap; //< records the pollinator's
                                                                             //< current performance info
                                                                             //< for each plant species
                                                                             //< (key is species id)

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

    /**
     * The distance in hex space that yields the maximum confidence of match when comparing a stimulus with a target
     */
    static float m_sVisMatchMinHexDistance;

    /**
     * The highest confidence of match when comparing a stimulus with a target, associated with distances of visMatchMinHexDistance or less
     */
    static float m_sVisMatchMaxConfidence;

    /**
     * The distance in hex space that yields the minimum confidence of match when comparing a stimulus with a target
     */
    static float m_sVisMatchMaxHexDistance;

    /**
     * The lowest confidence of match when comparing a stimulus with a target, associated with distances of visMatchMaxHexDistance or greater
     */
    static float m_sVisMatchMinConfidence;

    /**
     * Flags whether statics have been initialised from config file
     */
    static bool m_sbStaticsInitialised;
};

#endif /* _POLLINATOR_H */
