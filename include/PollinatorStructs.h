/**
 * @file
 *
 * Declaration of structs associated with Pollinators: 
 * PollinatorPerformanceInfo
 */

#ifndef _POLLINATORSTRUCTS_H
#define _POLLINATORSTRUCTS_H

#include "ReflectanceInfo.h"

/**
 * The PollinatorPerformanceInfo struct
 */
struct PollinatorPerformanceInfo {
    PollinatorPerformanceInfo() : numLandings(0), numPollinations(0) {}
    inline void reset() {numLandings = 0; numPollinations = 0;}

    int numLandings;
    int numPollinations;
};

/**
 * The VisualStimulusInfo struct
 * Defines data associated with pollinator's visual system and hexagon colour representation
 */
struct VisualStimulusInfo {
    VisualStimulusInfo() : mp(0), detectionProb(0.0), greenContrast(0.0), x(0.0), y(0.0) {}

    VisualStimulusInfo(MarkerPoint _mp, float _dp, float _gc, float _x, float _y)
        : mp(_mp), detectionProb(_dp), greenContrast(_gc), x(_x), y(_y)
    {}

    MarkerPoint mp;
    float detectionProb;
    float greenContrast;
	float x;
	float y;
};

#endif /* _POLLINATORSTRUCTS_H */
