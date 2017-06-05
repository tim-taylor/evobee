/**
 * @file
 *
 * Declaration of the Environment class
 */

#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <vector>
#include <memory>
#include "Patch.h"
#include "AbstractHive.h"
#include "Position.h"

using PatchVector = std::vector<Patch>;
using HivePtrVector = std::vector<std::shared_ptr<AbstractHive>>;
using PollinatorPtrVector = std::vector<Pollinator*>;

class FloweringPlant;


/**
 * The Environment class...
 */
class Environment {

public:
    Environment();
    ~Environment() {}

    /**
     * Helper function to calculate a patch's X and Y coordinates from its Index value
     * @param idx The Index value
     */
    iPos getPatchCoordsFromIdx(int idx);

    /**
     *
     */
    Patch& getPatch(int x, int y);
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
    bool inEnvironment(const fPos& pos) const {return inEnvironment((int)pos.x, (int)pos.y);}

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

private:
    void initialisePlants();     // private helper method used in constructor
    
    PatchVector   m_Patches;     ///< All patches are stored in a 1D vector for speed of access
    HivePtrVector m_Hives;       ///< Collection of all hives in the environment
    int           m_iNumPatches; ///< Number of patches (stored for convenience)
    int           m_iSizeX;      ///< Number of patches in X direction (int coords should be less than this)
    int           m_iSizeY;      ///< Number of patches in Y direction (int coords should be less than this)
    float         m_fSizeX;      ///< Number of patches in X direction (float coords should be less than this)
    float         m_fSizeY;      ///< Number of patches in Y direction (float coords should be less than this)

    PollinatorPtrVector m_AllPollinators; /** Aggregation of pointers to all Pollinators
                                           * NB The Pollinators are actually owened by
                                           * individual Hives
                                           */
};

#endif /* _ENVIRONMENT_H */
