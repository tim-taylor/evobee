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

/**
 * The Environment class...
 */
class Environment {

public:
    Environment();
    ~Environment() {}

    /// Helper function to calculate a patch's X and Y coordinates from its Index value
    /// @param idx The Index value
    /// @param retX Pointer to an int into which the X value will be returned
    /// @param retY Pointer to an int into which the Y value will be returned
    void getPatchCoordsFromIdx(int idx, int* retX, int* retY);

    /**
     *
     */
    Patch& getPatch(int x, int y);

    ///
    std::vector<Patch>& getPatches() {return m_Patches;};

private:
    void initialisePlants();      // private helper method used in constructor
    
    std::vector<Patch> m_Patches; ///< All patches are stored in a 1D vector for speed of access
    std::vector<std::shared_ptr<AbstractHive>>  m_Hives;   ///< Collection of all hives in the environment
    int m_iNumPatches;            ///< Number of patches (stored for convenience)
};

#endif /* _ENVIRONMENT_H */
