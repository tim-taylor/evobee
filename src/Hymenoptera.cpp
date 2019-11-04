/**
 * @file
 *
 * Implementation of the Hymenoptera class
 */

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include "Hymenoptera.h"
#include "PollinatorStructs.h"
#include "EvoBeeModel.h"


// Instantiate static data members
std::string Hymenoptera::m_sTypeNameStr{"HYM"};
std::vector<VisualStimulusInfo> Hymenoptera::m_sVisData;
MarkerPoint Hymenoptera::m_sVisDataMPMin = 1;
MarkerPoint Hymenoptera::m_sVisDataMPStep = 1;
MarkerPoint Hymenoptera::m_sVisDataMPMax = 1;
float Hymenoptera::m_sVisBaseProbLandTarget = 0.9;
float Hymenoptera::m_sVisProbLandNoTargetSetDelta = 0.2;
bool Hymenoptera::m_sbStaticsInitialised = false;


Hymenoptera::Hymenoptera(const PollinatorConfig& pc, AbstractHive* pHive) :
    Pollinator(pc, pHive)
{
    // first initialise the Hymenoptera class' static data relating to its visual system,
    // if this has not already been done
    if (!m_sbStaticsInitialised && pc.visDataDefined) {
        m_sVisDataMPMin = pc.visDataMPMin;
        m_sVisDataMPStep = pc.visDataMPStep;
        m_sVisDataMPMax = pc.visDataMPMax;
        m_sVisData = pc.visData;
        m_sVisBaseProbLandTarget = pc.visBaseProbLandTarget;
        m_sVisProbLandNoTargetSetDelta = pc.visProbLandNoTargetSetDelta;
        m_sbStaticsInitialised = true;
    }

    // initialise this instance's visual preference data
    std::normal_distribution<float> dist(0.0, pc.visProbLandNonTargetIndivStdDev);

    if (m_ConstancyType == PollinatorConstancyType::VISUAL) {
        for (MarkerPoint mp = m_sVisDataMPMin; mp <= m_sVisDataMPMax; mp += m_sVisDataMPStep)
        {
            float baseProbLandNonTargetInnate = getBaseProbLandNonTargetInnate(mp);
            float baseProbLandNonTargetIndivDelta = dist(EvoBeeModel::m_sRngEngine);
            float baseProbLandNonTarget = baseProbLandNonTargetInnate + baseProbLandNonTargetIndivDelta;
            m_VisualPreferences.emplace_back(mp, m_sVisBaseProbLandTarget, baseProbLandNonTarget);
        }
    }
}

Hymenoptera::Hymenoptera(const Hymenoptera& other) :
    Pollinator(other)
{
}

Hymenoptera::Hymenoptera(Hymenoptera&& other) noexcept :
    Pollinator(std::move(other))
{
}

void Hymenoptera::reset()
{
    Pollinator::reset();
    for (VisualPreferenceInfo& vpi : m_VisualPreferences)
    {
        vpi.reset();
    }
}


std::string Hymenoptera::getStateString() const
{
    return Pollinator::getStateString();
}


const std::string& Hymenoptera::getTypeName() const
{
    return m_sTypeNameStr;
}

float Hymenoptera::getMPDetectionProb(MarkerPoint mp) const
{
    float dp = 0.0;
    try
    {
        const VisualStimulusInfo& info = getVisStimInfoFromMP(mp);
        dp = info.detectionProb;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error in Hymenoptera::getMPDetectionProb(mp) with mp=" << mp << "." << std::endl
                  << "  " << e.what() << ". Aborting !" << std::endl;
        exit(1);
    }

    return dp;
}

const VisualStimulusInfo& Hymenoptera::getVisStimulusInfo(MarkerPoint mp) const
{
    // Define an empty VisualStimulusInfo object and return it at end of method to
    // please the compiler. If the method works correctly we'll actually return
    // the correct data within the try block and ignore this dummy object.
    static VisualStimulusInfo visInfoDummy;

    try
    {
        const VisualStimulusInfo& info = getVisStimInfoFromMP(mp);
        return info;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error in Hymenoptera::getVisStimulusInfo(mp) with mp=" << mp << "." << std::endl
                  << "  " << e.what() << ". Aborting !" << std::endl;
        exit(1);
    }

    return visInfoDummy;
}


// A helper method to calculate the index of a specific entry in the m_sVisData or m_VisualPreferences vectors
// that corresponds to the given marker point
std::size_t Hymenoptera::getVisualDataVectorIdx(MarkerPoint mp)
{
    assert(m_sbStaticsInitialised);

    if (mp < m_sVisDataMPMin)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisualDataVectorIdx:" << std::endl
            << "  Marker point " << mp << " below minimum value defined in config file (" << m_sVisDataMPMin << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    if ((mp - m_sVisDataMPMin) % m_sVisDataMPStep != 0)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisualDataVectorIdx:" << std::endl
            << "  Marker point " << mp << " is not at expected step size (" << m_sVisDataMPStep
            << ") above minimum value (" << m_sVisDataMPMin << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    if (mp > m_sVisDataMPMax)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisualDataVectorIdx:" << std::endl
            << "  Marker point " << mp << " is above maximum value defined in config file (" << m_sVisDataMPMax << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    return (std::size_t)((mp - m_sVisDataMPMin) / m_sVisDataMPStep);
}


const VisualStimulusInfo& Hymenoptera::getVisStimInfoFromMP(MarkerPoint mp)
{
    std::size_t idx = getVisualDataVectorIdx(mp);
    return m_sVisData.at(idx);
}

VisualPreferenceInfo& Hymenoptera::getVisPrefInfoFromMP(MarkerPoint mp)
{
    std::size_t idx = getVisualDataVectorIdx(mp);
    return m_VisualPreferences.at(idx);
}

const VisualPreferenceInfo& Hymenoptera::getVisPrefInfoFromMPConst(MarkerPoint mp) const
{
    std::size_t idx = getVisualDataVectorIdx(mp);
    return m_VisualPreferences.at(idx);
}

float Hymenoptera::getBaseProbLandNonTargetInnate(MarkerPoint mp)
{
    return getVisStimInfoFromMP(mp).baseProbLandNonTargetInnate;
}


// Determine whether the pollinator will decide to land on the given flower,
// using its visual perception
bool Hymenoptera::isVisitCandidateVisual(Flower* pFlower) const
{
    bool bIsVisitCandidate = false;

    MarkerPoint flowerMP = pFlower->getMarkerPoint();

    float detectionProb = getMPDetectionProb(flowerMP);

    bool bDetected = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < detectionProb);

    if (bDetected)
    {
        const VisualPreferenceInfo& visPrefInfo = getVisPrefInfoFromMPConst(flowerMP);

        if (m_TargetMP == NO_MARKER_POINT)
        {
            float landProb = visPrefInfo.getProbLandNonTarget() + m_sVisProbLandNoTargetSetDelta;
            bIsVisitCandidate = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < landProb);
        }
        else
        {
            bool bLooksLikeTarget = matchesTargetMP(pFlower->getReflectanceInfo());

            if (bLooksLikeTarget)
            {
                bIsVisitCandidate = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < visPrefInfo.getProbLandTarget());
            }
            else
            {
                bIsVisitCandidate = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < visPrefInfo.getProbLandNonTarget());
            }
        }
    }

    return bIsVisitCandidate;
}


// Update the pollinator's visual preference info after each visit to a flower.
void Hymenoptera::updateVisualPreferences(const Flower* pFlower, int nectarCollected)
{
    //////////////////////////////////////////////////////////
    // Update the pollinator's preferences on the basis of the latest flower visit and rewards received

    const ReflectanceInfo& flowerReflectance = pFlower->getReflectanceInfo();
    MarkerPoint flowerMP = flowerReflectance.getMarkerPoint();
    VisualPreferenceInfo& visPrefInfo = getVisPrefInfoFromMP(flowerMP);
    bool bLooksLikeTarget = matchesTargetMP(flowerReflectance); // TODO do we wnat "looks like Target" or "IS Target" here??

    if (nectarCollected > 0)
    {
        // if the visitied flower was rewarding...
        if (bLooksLikeTarget) {
            visPrefInfo.incrementProbLandTarget(0.01);  // TODO must make a param to specify learning rate
        }
        else {
            visPrefInfo.incrementProbLandNonTarget(0.01);
        }
    }
    else
    {
        // if the visited flower was not rewarding...
        if (bLooksLikeTarget) {
            visPrefInfo.decrementProbLandTarget(0.01);  // TODO must make a param to specify learning rate
        }
        else {
            visPrefInfo.decrementProbLandNonTarget(0.01);
        }
    }

    //////////////////////////////////////////////////////////
    // Consider switching to a new target based upon newly updated preferences

    VisualPreferenceInfo& vpiCurrentTarget = getVisPrefInfoFromMP(m_TargetMP);
    float maxProbLandNonTarget = 0.0;
    VisualPreferenceInfo* pVpiMaxNonTarget = nullptr;
    for (auto& vpi : m_VisualPreferences)
    {
        if ((vpi.getMarkerPoint() != m_TargetMP) && (vpi.getProbLandNonTarget() > maxProbLandNonTarget))
        {
            pVpiMaxNonTarget = &vpi;
            maxProbLandNonTarget = vpi.getProbLandNonTarget();
        }
    }

    if ((pVpiMaxNonTarget != nullptr) && (maxProbLandNonTarget > vpiCurrentTarget.getProbLandTarget()))
    {
        m_TargetMP = pVpiMaxNonTarget->getMarkerPoint();
        pVpiMaxNonTarget->setTarget();
        vpiCurrentTarget.setNonTarget();
    }

    /*
    ...Now call Procedure: update target bool
    Iterate through the bee pref% array
    Get the target% for the current target flower

    Iterate through the bee pref% array
    For each flower in the array that is not the target
        Get the non-target% for the flower and find the non-target with the highest non-target% (Randomly? handle multiple flowers with equal highest non-target%)

    if the highest non-target% > target% for the current target flower then
        set the non-target flower's target%=baseTarget% // return its value to the (high) default
        set the non-target flower's isCurrentTarget=true // make it the new target
        set the (old) target flower's non-target% = the (old) target flower's target% // i.e. it used to have a high preference, keep it, but now use it as the non-target preference
        set the (old) target flower's isCurrentTarget=false // make it now just a non-target
    */


    //////////////////////////////////////////////////////////
    // Attenuate preferences for non-target flowers

    for (auto& vpi : m_VisualPreferences)
    {
        //if (!isRecentlyVisited(vpi.getMarkerPoint()))
        //{
        vpi.decrementProbLandNonTarget(0.01); // TODO figure out what this should be and use a param - and for now we're decrementing
            // prob land non target for all stimuli, even those not recently visited
        //}
    }

    /*
    Iterate through the bee pref% array
    For every flower not in the recently visited list
    decrement its non-target% (i.e. forget the preference for it a little << not sure how this should work yet)
    */
}