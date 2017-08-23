/**
 * @file
 *
 * Implementation of the tools methods
 */

#include <cmath>
#include "tools.h"

namespace EvoBee
{
    /**
     * Utility function to compare two floating point numbers for (near enough) equality
     */
    bool equal(float a, float b)
    {
        return fabs(a - b) < FLOAT_COMPARISON_EPSILON;
    }

} // namespace EvoBee
