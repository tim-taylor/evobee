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
using HiveVector = std::vector<std::shared_ptr<AbstractHive>>;

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

    /**
     *
     */
    PatchVector& getPatches() {return m_Patches;}

    /**
     *
     */
    HiveVector& getHives() {return m_Hives;}

    /*
     * Returns a random float position within the environment
     */
    fPos getRandomPositionF() const;

private:
    void initialisePlants();      // private helper method used in constructor
    
    PatchVector m_Patches; ///< All patches are stored in a 1D vector for speed of access
    HiveVector  m_Hives;   ///< Collection of all hives in the environment
    int m_iNumPatches;     ///< Number of patches (stored for convenience)
};

#endif /* _ENVIRONMENT_H */
