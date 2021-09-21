/**
 * @file
 *
 * Declaration of structs associated with Pollinators:
 * PollinatorPerformanceInfo, VisualStimulusInfo, VisualPreferenceInfo
 */

#ifndef _POLLINATORSTRUCTS_H
#define _POLLINATORSTRUCTS_H

#include "PollinatorEnums.h"
#include "ReflectanceInfo.h"
#include "Flower.h"


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
    VisualStimulusInfo() : id(-1), lambda(0), detectionProb(0.0), greenContrast(0.0), hexx(0.0), hexy(0.0),
                           purex(0.0), purey(0.0), baseProbLandNonTargetInnate(0.0), aux_id(-1)
    {}

    VisualStimulusInfo(Wavelength _lambda, float _dp, float _gc, float _hexx, float _hexy, float _baseProbLandNonTargetInnate)
        : id(-1), lambda(_lambda), detectionProb(_dp), greenContrast(_gc), hexx(_hexx), hexy(_hexy), purex(0.0f), purey(0.0f),
          baseProbLandNonTargetInnate(_baseProbLandNonTargetInnate), aux_id(-1)
    {}

    VisualStimulusInfo(int _id, Wavelength _lambda, float _dp, float _gc, float _hexx, float _hexy, float _purex, float _purey,
                       float _baseProbLandNonTargetInnate, int _aux_id)
        : id(_id), lambda(_lambda), detectionProb(_dp), greenContrast(_gc), hexx(_hexx), hexy(_hexy), purex(0.0f), purey(0.0f),
          baseProbLandNonTargetInnate(_baseProbLandNonTargetInnate), aux_id(_aux_id)
    {}

    Wavelength getWavelength() const {return lambda;}

    int id;                 ///< id used to refer to this entry (only used under ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS)
    Wavelength lambda;      ///< this represents a MarkerPoint under ColourSystem::REGULAR_MARKER_POINTS,
                            ///<   or a dominant wavelength under ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS
    float detectionProb;
    float greenContrast;
	float hexx;             ///< x coordinate of position of this stimulus in hexagon colour space
	float hexy;             ///< y coordinate of position of this stimulus in hexagon colour space
    float purex;            ///< x coordinate of point on pure spectral line corresponding to dominant wavelength of this stimulus
    float purey;            ///< y coordinate of point on pure spectral line corresponding to dominant wavelength of this stimulus
    float baseProbLandNonTargetInnate;
    int aux_id;             ///< An auxiliary ID such as a FReD database ID (optional, used only in output logs)
};


/**
 * The VisualPreferenceInfo struct
 * Defines data associated with a pollinator's current visual preferences and landing probabilities.
 * This data may vary between individual pollinators of the same species.
 */
struct VisualPreferenceInfo {
    VisualPreferenceInfo() : lambda(NO_MARKER_POINT), pVisStimInfo(nullptr),
        probLandTarget(1.0), probLandNonTarget(1.0), baseProbLandTarget(1.0), baseProbLandNonTarget(1.0) {}

    VisualPreferenceInfo(Wavelength _lambda, float _baseProbLandTarget, float _baseProbLandNonTarget)
        : lambda(_lambda), pVisStimInfo(nullptr),
          baseProbLandTarget((_baseProbLandTarget < 0.0) ? 0.0 : ((_baseProbLandTarget > 1.0) ? 1.0 : _baseProbLandTarget)),
          baseProbLandNonTarget((_baseProbLandNonTarget < 0.0) ? 0.0 : ((_baseProbLandNonTarget > 1.0) ? 1.0 : _baseProbLandNonTarget))
    {
        probLandTarget = baseProbLandTarget;
        probLandNonTarget = baseProbLandNonTarget;
    }

    VisualPreferenceInfo(const VisualStimulusInfo* _pVisStimInfo, float _baseProbLandTarget, float _baseProbLandNonTarget)
        : lambda(_pVisStimInfo->getWavelength()), pVisStimInfo(_pVisStimInfo),
          baseProbLandTarget((_baseProbLandTarget < 0.0) ? 0.0 : ((_baseProbLandTarget > 1.0) ? 1.0 : _baseProbLandTarget)),
          baseProbLandNonTarget((_baseProbLandNonTarget < 0.0) ? 0.0 : ((_baseProbLandNonTarget > 1.0) ? 1.0 : _baseProbLandNonTarget))
    {
        probLandTarget = baseProbLandTarget;
        probLandNonTarget = baseProbLandNonTarget;
    }

    const VisualStimulusInfo* getVisualStimulusInfoPtr() const {return pVisStimInfo;}
    Wavelength getWavelength() const {
        return lambda;
        /*
        if (pVisStimInfo==nullptr) {
            throw std::runtime_error("VisualPreferenceInfo::getWavelength() called when pVisStimInfo is not set");
        }
        else {
            return pVisStimInfo->lambda;
        }
        */
    }

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

    void setAsTarget()
    {
        probLandTarget = baseProbLandTarget;
    }

    void setAsNonTarget()
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

private:
    Wavelength lambda;          ///> The info in this struct relates to a flower stimulus with this characteristic wavelength
    const VisualStimulusInfo* pVisStimInfo;
                                ///> A (non-owning) pointer to the visual stimulus information structure
                                ///> describing what stimulus this preference relates to
                                ///> Careful! This may not be set if ColourSystem==REGULAR_MARKER_POINTS

public:
    float probLandTarget;       ///> The pollinator's current probability of landing on a flower with this marker point
                                ///> if it thinks the marker point matches its current target. Expressed as a number between 0.0 - 1.0.
                                ///> Note that prefTarget and prefNonTarget are independent and do not need to sum to 1.0.

    float probLandNonTarget;    ///> The pollinator's current probability of landing on a flower with this marker point
                                ///> if it thinks the marker point does not match its current target. Expressed as a number between 0.0 - 1.0.
                                ///> Note that prefTarget and prefNonTarget are independent and do not need to sum to 1.0.

    const float baseProbLandTarget;     ///> The pollinators base value for probLandTarget before any learning has occured (or after a reset)
    const float baseProbLandNonTarget;  ///> This pollinator's base value for probLandNonTarget before any learning has occured (or after a reset)
};


struct PollinatorLatestAction
{
    PollinatorLatestAction() :
        stepnum(0), status(PollinatorCurrentStatus::NO_FLOWER_SEEN),
        pFlower(nullptr), rewardReceived(0), bJudgedToMatchTarget(false)
        {};

    void update(int _step, PollinatorCurrentStatus _status, Flower* _pFlower, int _reward, bool _matchedTarget = false) {
        stepnum = _step;
        status = _status;
        pFlower = _pFlower;
        rewardReceived = _reward;
        bJudgedToMatchTarget = _matchedTarget;
    };

    int stepnum;
    PollinatorCurrentStatus status;
    Flower* pFlower;
    int rewardReceived;
    bool bJudgedToMatchTarget;
};


#endif /* _POLLINATORSTRUCTS_H */
