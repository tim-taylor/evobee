/**
 * @file
 *
 * Implementation of the tools methods
 */

#include <cmath>
#include "tools.h"

namespace EvoBee
{
    /*
     * Utility function to compare two floating point numbers for (near enough) equality
     */
    bool equal(float a, float b)
    {
        return fabs(a - b) < FLOAT_COMPARISON_EPSILON;
    }

    /*
     * Return the square of the distance between two points
     */
    float distanceSq(const fPos& pos1, const fPos& pos2)
    {
        return (((pos1.x - pos2.x)*(pos1.x - pos2.x)) + ((pos1.y - pos2.y)*(pos1.y - pos2.y)));
    }

} // namespace EvoBee
