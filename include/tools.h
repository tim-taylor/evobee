/**
 * @file
 *
 * A miscellaneous collection of general tools
 */

#ifndef _TOOLS_H
#define _TOOLS_H

#include <cmath>
#include "Position.h"

namespace EvoBee
{
    constexpr float FLOAT_COMPARISON_EPSILON = 0.00000001f;
    constexpr float SMALL_FLOAT_NUMBER = 0.00001f;

    constexpr double PI = 4.0*std::atan(1);
    constexpr double TWOPI = 2.0*PI;

    /**
     * Utility function to compare two floating point numbers for (near enough) equality
     */
    bool equal(float a, float b);

    /**
     * Return the square of the distance between two points
     */
    float distanceSq(const fPos& pos1, const fPos& pos2);

    /**
     * Return a random variate from a Levy distribution
     * @param max The maximum possible return value
     * @param scale Determines positions of the peak of the Levy PDF (must be > 0.0)
     */
    float randomLevy(float max = 50.0, float scale = 1.0);

    /**
     * Return a random variate from a standard Cauchy distribution
     * @param min The minimum possible return value
     * @param max The maximum possible return value
     */
    float randomCauchy(float min = 0.5, float max = 20.0);

} // namespace EvoBee

#endif /* _TOOLS_H */
