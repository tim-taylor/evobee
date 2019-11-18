/**
 * @file
 *
 * Declaration of structs associated with Pollinators:
 * PollinatorPerformanceInfo, VisualStimulusInfo, VisualPreferenceInfo
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
 * Defines data associated with pollinator's visual system and hexagon colour representation.
 * This data is static (constant) for a given species of pollinator.
 */
struct VisualStimulusInfo {
    VisualStimulusInfo() : mp(0), detectionProb(0.0), greenContrast(0.0), x(0.0), y(0.0), baseProbLandNonTargetInnate(0.0) {}

    VisualStimulusInfo(MarkerPoint _mp, float _dp, float _gc, float _x, float _y, float _baseProbLandNonTargetInnate)
        : mp(_mp), detectionProb(_dp), greenContrast(_gc), x(_x), y(_y), baseProbLandNonTargetInnate(_baseProbLandNonTargetInnate)
    {}

    MarkerPoint mp;
    float detectionProb;
    float greenContrast;
	float x;
	float y;
    float baseProbLandNonTargetInnate;
};


/**
 * The VisualPreferenceInfo struct
 * Defines data associated with a pollinator's current visual preferences and landing probabilities.
 * This data may vary between individual pollinators of the same species.
 */
struct VisualPreferenceInfo {
    VisualPreferenceInfo() : mp(0), probLandTarget(1.0), probLandNonTarget(1.0), baseProbLandTarget(1.0), baseProbLandNonTarget(1.0) {}

    VisualPreferenceInfo(MarkerPoint _mp, float _baseProbLandTarget, float _baseProbLandNonTarget)
        : mp(_mp),
          baseProbLandTarget((_baseProbLandTarget < 0.0) ? 0.0 : ((_baseProbLandTarget > 1.0) ? 1.0 : _baseProbLandTarget)),
          baseProbLandNonTarget((_baseProbLandNonTarget < 0.0) ? 0.0 : ((_baseProbLandNonTarget > 1.0) ? 1.0 : _baseProbLandNonTarget))
    {
        probLandTarget = baseProbLandTarget;
        probLandNonTarget = baseProbLandNonTarget;
    }

    MarkerPoint getMarkerPoint() const  {return mp;}
    float getProbLandTarget() const     {return probLandTarget;}
    float getProbLandNonTarget() const  {return probLandNonTarget;}

    void incrementProbLandTarget(float delta)
    {
        probLandTarget += delta;
        constrainProbLandTarget();

    }

    void decrementProbLandTarget(float delta)
    {
        probLandTarget -= delta;
        constrainProbLandTarget();
    }

    void incrementProbLandNonTarget(float delta)
    {
        probLandNonTarget += delta;
        constrainProbLandNonTarget();
    }

    void decrementProbLandNonTarget(float delta)
    {
        probLandNonTarget -= delta;
        constrainProbLandNonTarget();
    }

    void setTarget()
    {
        probLandTarget = baseProbLandTarget;
    }

    void setNonTarget()
    {
        probLandNonTarget = probLandTarget;
    }

    void constrainProbLandTarget()
    {
        if (probLandTarget > baseProbLandTarget)
        {
            probLandTarget = baseProbLandTarget;
        }
        else if (probLandTarget < baseProbLandNonTarget)
        {
            probLandTarget = baseProbLandNonTarget;
        }
    }

    void constrainProbLandNonTarget()
    {
        if (probLandNonTarget < baseProbLandNonTarget)
        {
            probLandNonTarget = baseProbLandNonTarget;
        }
        else if (probLandNonTarget > baseProbLandTarget)
        {
            probLandNonTarget = baseProbLandTarget;
        }
    }

    void reset()
    {
        probLandTarget = baseProbLandTarget;
        probLandNonTarget = baseProbLandNonTarget;
    }

//private:
    MarkerPoint mp;             ///> The following info relates to a flower stimulus with this marker point

    float probLandTarget;       ///> The pollinator's current probability of landing on a flower with this marker point
                                ///> if it thinks the marker point matches its current target. Expressed as a number between 0.0 - 1.0.
                                ///> Note that prefTarget and prefNonTarget are independent and do not need to sum to 1.0.

    float probLandNonTarget;    ///> The pollinator's current probability of landing on a flower with this marker point
                                ///> if it thinks the marker point does not match its current target. Expressed as a number between 0.0 - 1.0.
                                ///> Note that prefTarget and prefNonTarget are independent and do not need to sum to 1.0.

    const float baseProbLandTarget;     ///> The pollinators base value for probLandTarget before any learning has occured (or after a reset)
    const float baseProbLandNonTarget;  ///> This pollinator's base value for probLandNonTarget before any learning has occured (or after a reset)
};

#endif /* _POLLINATORSTRUCTS_H */
