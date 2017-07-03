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
#include "Position.h"

using PatchVector = std::vector<Patch>;
using HivePtrVector = std::vector<std::shared_ptr<AbstractHive>>;
using PollinatorPtrVector = std::vector<Pollinator*>;

class FloweringPlant;
class EvoBeeModel;


/**
 * The Environment class...
 */
class Environment {

public:
    Environment(EvoBeeModel* pModel);
    ~Environment() {}

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
     * Search for flowering plants within range of given position.
     * If any found, return a pointer to the closest one, otherwise
     * return nullptr
     */
    FloweringPlant* findClosestFloweringPlant(const fPos& pos);

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
    
    PatchVector   m_Patches;     ///< All patches are stored in a 1D vector for speed of access
    HivePtrVector m_Hives;       ///< Collection of all hives in the environment
    int           m_iNumPatches; ///< Num patches (stored for convenience)
    int           m_iSizeX;      ///< Num patches in X dir (int coords should be less than this)
    int           m_iSizeY;      ///< Num patches in Y dir (int coords should be less than this)
    float         m_fSizeX;      ///< Num patches in X dir (float coords should be less than this)
    float         m_fSizeY;      ///< Num patches in Y dir (float coords should be less than this)

    PollinatorPtrVector m_AllPollinators; /** Aggregation of pointers to all Pollinators
                                           * NB The Pollinators are actually owened by
                                           * individual Hives
                                           */

    const EvoBeeModel* m_pModel;                                           
};

#endif /* _ENVIRONMENT_H */
