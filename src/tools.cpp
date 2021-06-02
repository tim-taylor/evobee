/**
 * @file
 *
 * Implementation of the tools methods
 */

#include <cmath>
#include <algorithm>
#include <iostream>
#include "tools.h"
#include "EvoBeeModel.h"

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

    /*
     * Return a random variate from a Levy distribution
     *
     * This is an implementation of the rejection sampling method. For details see
     * https://en.wikipedia.org/wiki/Rejection_sampling
     * and
     * https://en.wikipedia.org/wiki/L%C3%A9vy_distribution
     *
     * @param max The maximum possible return value
     * @param scale Determines positions of the peak of the Levy PDF (must be > 0.0)
     *
     */
    float randomLevy(float max /*=50.0*/, float scale /*=1.0*/)
    {
        float x, y, pdf, ymax;

        if (scale <= 0.0) {
            throw std::runtime_error("In randomLevy(): scale parameter must be greater than 0.0!");
        }

        // find a suitable maximum value for the proposal distribution, based upon the
        // maximum of the Levy PDF at difference values of the scale parameter
        if (scale < 1.0) {
            ymax = 1.0;
        } else if (scale < 2.0) {
            ymax = 0.5;
        } else {
            ymax = 0.25;
        }

        do {
            x = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) * max;
            y = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) * ymax;
            pdf = std::sqrt(scale/EvoBee::TWOPI) * std::exp(-scale/(2.0*x)) / (std::pow(x,1.5));
        } while (y > pdf);

        return x;
    }

    /*
     * Return a random variate from a standard Cauchy distribution (with scale=1.0
     * and shift=0.0).
     *
     * A Cauchy distribution is a stable distribution with a PDF f(x) is approximately
     * proportional to x^-2. To be exact, the PDF of the standard Cauchy distribution
     * is f(x)=1/(PI.(1+x^2)). It is therefore a suitable representation of the
     * Levy distribution with an inverse-square law tail that Reynolds et al (2007)
     * found to be a good fit to observations of real honeybee flight paths.
     *
     * To adjust the distribution to be suitable for use in our simulations, this method
     * always returns a positive value (by taking the absolute value of the random
     * variate returned from the Cauchy distribution). Furthermore, we define
     * a minimum and maximum value that this method can return. Values below the minimum
     * or above the maximum are replaced with the minimum allowed value.
     *
     * The maximum has a default value of 20.0 perceptual distance units (pdus), which
     * corresponds to 14m. A honeybee has a typical maximum speed of ~25kmh = 7m/s.
     * Hence 20pdus is the maximum a honeybee might be able to fly in about 2 seconds.
     *
     * References:
     * - Andrew M. Reynolds et al. "Displaced Honey Bees Perform Optimal Scale-Free
     *   Search Flights", Ecology, 88(8), 2007, pp. 1955-1961.
     * - Oliver C. Ibe, "Elements of Random Walk and Diffusion Processes" (Chapter 8,
     *   "Levy Walk"), John Wiley & Sons, 2013.
     *
     * @param min The minimum possible return value
     * @param max The maximum possible return value
     *
     */
    float randomCauchy(float min /*=0.5*/, float max /*=20.0*/)
    {
        float length = std::abs(EvoBeeModel::m_sCauchyProbDistrib(EvoBeeModel::m_sRngEngine));

        if (length < min || length > max) {
            length = min;
        }

        return length;
    }

} // namespace EvoBee
