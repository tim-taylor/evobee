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
    float randomLevy(float max = 50.0, float scale = 1.0)
    {
        float x, y, pdf, ymax;

        if (scale <= 0.0) {
            throw std::runtime_error("In randomLevy(): scale parameter must be greate than 0.0!");
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

    float randomLevy2()
    {
        /*
        // see: https://stackoverflow.com/questions/19208502/levy-walk-simulation-in-r
        float alpha = 2.0;
        float fmin = 1.0;
        float fmax = 20.0;
        std::uniform_real_distribution<float> dist(fmin, fmax);
        float length = std::pow(dist(EvoBeeModel::m_sRngEngine), (-1.0/alpha));
        */

        //
        // see https://dspace.mit.edu/bitstream/handle/1721.1/83632/864715848-MIT.pdf
        float max = 50.0;
        //float mu = 2.0;
        float lmin = 1.0;
        //float u = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine);
        float umax = 0.95;
        std::uniform_real_distribution<float> dist(0.0, umax);
        float u = dist(EvoBeeModel::m_sRngEngine);
        //float length = lmin * std::pow(1.0-u, 1.0/(-mu+1.0)); // general case for any mu
        float length = lmin * 1.0 / (1.0-u); // for mu = 2.0
        length = std::min(length, max);
        //

        std::cout << length << std::endl;

        return length;
    }

} // namespace EvoBee
