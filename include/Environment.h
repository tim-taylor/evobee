/**
 * @file
 *
 * Declaration of the Environment class
 */

#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <vector>
#include <memory>
#include <cmath>
#include "Patch.h"
#include "AbstractHive.h"
#include "PlantTypeDistributionConfig.h"
#include "Position.h"

using PatchVector = std::vector<Patch>;
using HivePtrVector = std::vector<std::shared_ptr<AbstractHive>>;
using PollinatorPtrVector = std::vector<Pollinator*>;

class FloweringPlant;
class EvoBeeModel;


/**
 * The LocalDensityConstraint struct
 * Used for keeping track of constraints on local plant densities during the
 * reproduction process
 */
struct LocalDensityConstraint {

    // constructor
    LocalDensityConstraint(const PlantTypeDistributionConfig& _ptdcfg) :
        ptdcfg(_ptdcfg),
        curPlants(0)
    {
        int w = ptdcfg.areaBottomRight.x - ptdcfg.areaTopLeft.x + 1;
        int h = ptdcfg.areaBottomRight.y - ptdcfg.areaTopLeft.y + 1;
        maxPlants = (int)(ptdcfg.reproLocalDensityMax * (float)(w * h));
    }

    // helper methods
    bool posInArea(const iPos& pos) const;

    // data members
    const PlantTypeDistributionConfig& ptdcfg;
    unsigned int maxPlants;
    unsigned int curPlants;
};


/**
 * The Environment class...
 */
class Environment {

public:
    Environment(EvoBeeModel* pModel);
    //~Environment() {}

    /**
     * Initialise a new generation
     */
    void initialiseNewGeneration();

    /**
     * Helper function to calculate a patch's X and Y coordinates from its Index value
     * @param idx The Index value
     */
    iPos getPatchCoordsFromIdx(int idx);

    /**
     *
     */
    Patch& getPatch(int x, int y);
    Patch& getPatch(iPos pos) {return getPatch(pos.x, pos.y);}
    Patch& getPatch(fPos pos) {return getPatch((int)pos.x, (int)pos.y);}


    /**
     *
     */
    PatchVector& getPatches() {return m_Patches;}

    /**
     *
     */
    HivePtrVector& getHives() {return m_Hives;}


    /**
     * Get integer size of Environment in x direction
     * (coordinates should be less than this value)
     */
    int getSizeXi() const {return m_iSizeX;}

    /**
     * Get integer size of Environment in y direction
     * (coordinates should be less than this value)
     */
    int getSizeYi() const {return m_iSizeY;}

    /**
     * Get float size of Environment in x direction
     * (coordinates should be less than this value, this is upper bound)
     */
    float getSizeXf() const {return m_fSizeX;}

    /**
     * Get float size of Environment in y direction
     * (coordinates should be less than this value, this is upper bound)
     */
    float getSizeYf() const {return m_fSizeY;}

    /**
     * Checks whether the given position is within the bounds of the environment
     */
    bool inEnvironment(int x, int y) const;

    /**
     * Checks whether the given position is within the bounds of the environment
     */
    bool inEnvironment(const iPos& pos) const {return inEnvironment(pos.x, pos.y);}

    /**
     * Checks whether the given position is within the bounds of the environment
     */
    bool inEnvironment(const fPos& pos) const {return inEnvironment(std::floor(pos.x),std::floor(pos.y));}

    /**
     * Checks whether the given positionis within the specified area
     */
    static bool inArea(const iPos& pos, const iPos& areaTopLeft, const iPos& areaBottomRight);

    /**
     * Search for flowering plants within range of given position
     * withing the Moore neighbourhood of local patches.
     * If any found, return a pointer to the nearest one, otherwise
     * return nullptr.
     *
     * @param fRadius specifies the meximum radius of the search, and should
     * be set between 0.0 and 1.0 (values above this mean that the
     * search might be clipped if the specified pos is not in the dead
     * centre of the central patch). If fRadius is set to a negative
     * value, it is ignored, and all flowers within the local Moore
     * neihbourhood of patches are considered (but not that this leads
     * to an asymmetric search radius in different directions, as the
     * patches are squares). fRadius takes a default value of 1.0.
     *
     * @param excludeCurrentPos determines whether to return a plant that
     * is in exactly the specified position. This is excluded by default,
     * as we are usually not interested in a plant that a pollinator is
     * currently sitting on.
     */
    FloweringPlant* findNearestFloweringPlant(const fPos& pos,
        float fRadius = 1.0,
        bool excludeCurrentPos = true);

    /**
     * Search for flowers in the local patch and its 8 closest neighbours
     * (Moore neighbourhood), and return a pointer to the closest flower found
     * that is not in the supplied list of excluded flowers, or nullptr if none found.
     *
     * The third parameter (default value = 1.0) specifies a maximum search radius.
     * If this is given a negative value, then it is ignored, and the closest flower
     * withiin the Moore neighbourhood will be returned regardless of distance (but
     * note that this means that the search radius is effectively asymmetric in
     * different directions because the patches are squares). If the radius is set
     * to a positive value, then only flowers within that distance of fpos will be
     * considered. Therefore, if fRadius is positive, it only really makes sense for
     * it to have a value of <= 1.0, as above 1.0 the search might be clipped depending
     * upon the location of fpos within the central patch.
     */
    Flower* findNearestUnvisitedFlower(const fPos &fpos,
        const std::vector<unsigned int>& excludeIdVec,
        float fRadius = 1.0,
        bool excludeCurrentPos = true);

    /*
     * Returns a random float position within the environment
     */
    fPos getRandomPositionF() const;

    /*
     * Returns a random float position within the specified area.
     * Note that the (int) bottomright param specifies an inclusive range,
     * so the returned position may be in up to and including that patch position
     */
    fPos getRandomPositionF(const iPos& topleft, const iPos& bottomright) const;

    /**
     * Return (int) coordinates of the Patch in which the given (float) position
     * belongs
     */
    static iPos getPatchCoordFromFloatPos(const fPos& fpos);

    /**
     * Returns the fraction of all plants that are currently pollinated
     */
    float getPollinatedFrac() const;

    /**
     * Add a pointer to a pollinator to the Environment's aggregate list of all
     * pollinators. This is called in the Hive constructor, and the individual
     * Hives actually own the Pollinator objects.
     */
    void addPollinatorToAggregateList(Pollinator* pPol) {m_AllPollinators.push_back(pPol);}

    /**
     * Return a vector of pointers to all pollinators from all hives
     */
    PollinatorPtrVector& getAllPollinators() {return m_AllPollinators;}

    /**
     * Return a pointer to the EvoBeeModel
     */
    const EvoBeeModel* getModel() const {return m_pModel;}


private:
    void initialisePlants();     // private helper method used in constructor

    // private methods for keeping track of local density limits during reproduction
    void initialiseLocalDensityCounts();
    void resetLocalDensityCounts();
    bool localDensityLimitReached(const iPos& newPatchPos) const;
    void incrementLocalDensityCount(const iPos& newPatchPos);

    PatchVector   m_Patches;     ///< All patches are stored in a 1D vector for speed of access
    HivePtrVector m_Hives;       ///< Collection of all hives in the environment
    int           m_iNumPatches; ///< Num patches (stored for convenience)
    int           m_iSizeX;      ///< Num patches in X dir (int coords should be less than this)
    int           m_iSizeY;      ///< Num patches in Y dir (int coords should be less than this)
    float         m_fSizeX;      ///< Num patches in X dir (float coords should be less than this)
    float         m_fSizeY;      ///< Num patches in Y dir (float coords should be less than this)

    PollinatorPtrVector m_AllPollinators; /** Aggregation of pointers to all Pollinators
                                           *  NB The Pollinators are actually owened by
                                           *  individual Hives
                                           */

    std::vector<LocalDensityConstraint> m_LocalDensityConstraints; ///< List of local plant density
                                                                   /// constraints, as defined by those
                                                                   /// PlantTypeDistributions for which
                                                                   /// reproLocalDensityConstrained=true
    const EvoBeeModel* m_pModel;
};

#endif /* _ENVIRONMENT_H */
