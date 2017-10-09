/**
 * @file
 *
 * A miscellaneous collection of general tools
 */

#ifndef _TOOLS_H
#define _TOOLS_H

#include "Position.h"

namespace EvoBee
{
    constexpr float FLOAT_COMPARISON_EPSILON = 0.00000001f;
    constexpr float SMALL_FLOAT_NUMBER = 0.00001f;

    /**
     * Utility function to compare two floating point numbers for (near enough) equality
     */
    bool equal(float a, float b);

    /**
     * Return the square of the distance between two points
     */
    float distanceSq(const fPos& pos1, const fPos& pos2);

} // namespace EvoBee

#endif /* _TOOLS_H */
