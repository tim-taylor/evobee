/**
 * @file
 *
 * A miscellaneous collection of general tools
 */

#ifndef _TOOLS_H
#define _TOOLS_H

#include <cmath>

namespace EvoBee
{
    constexpr float FLOAT_COMPARISON_EPSILON = 0.00000001;

    /**
     * Utility function to compare two floating point numbers for (near enough) equality
     */
    bool equal(float a, float b)
    {
        return fabs(a - b) < FLOAT_COMPARISON_EPSILON;
    }

} // namespace EvoBee

#endif /* _TOOLS_H */
