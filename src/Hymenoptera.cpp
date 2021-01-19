/**
 * @file
 *
 * Implementation of the Hymenoptera class
 */

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <cassert>
#include "Hymenoptera.h"
#include "PollinatorStructs.h"
#include "EvoBeeModel.h"
#include "ModelParams.h"
#include "tools.h"
#include "giurfa.h"


// Instantiate static data members
std::string Hymenoptera::m_sTypeNameStr{"HYM"};
std::vector<VisualStimulusInfo> Hymenoptera::m_sVisData;
MarkerPoint Hymenoptera::m_sVisDataMPMin = 1;
MarkerPoint Hymenoptera::m_sVisDataMPStep = 1;
MarkerPoint Hymenoptera::m_sVisDataMPMax = 1;
float Hymenoptera::m_sVisBaseProbLandTarget = 0.9;
float Hymenoptera::m_sVisProbLandNoTargetSetDelta = 0.2;
float Hymenoptera::m_sVisProbLandIncrementOnReward = 0.01;
float Hymenoptera::m_sVisProbLandDecrementOnNoReward = 0.01;
float Hymenoptera::m_sVisProbLandDecrementOnUnseen = 0.005;
bool Hymenoptera::m_sbVisTargetExactMatchOnly = false;
bool Hymenoptera::m_sbStaticsInitialised = false;


Hymenoptera::Hymenoptera(const PollinatorConfig& pc, AbstractHive* pHive) :
    Pollinator(pc, pHive)
{
    // first initialise the Hymenoptera class' static data relating to its visual system,
    // if this has not already been done
    if ((!m_sbStaticsInitialised) && pc.visDataDefined) {
        m_sVisDataMPMin = pc.visDataMPMin;
        m_sVisDataMPStep = pc.visDataMPStep;
        m_sVisDataMPMax = pc.visDataMPMax;
        m_sVisData = pc.visData;
        m_sVisBaseProbLandTarget = pc.visBaseProbLandTarget;
        m_sVisProbLandNoTargetSetDelta = pc.visProbLandNoTargetSetDelta;
        m_sVisProbLandIncrementOnReward = pc.visProbLandIncrementOnReward;
        m_sVisProbLandDecrementOnNoReward = pc.visProbLandDecrementOnNoReward;
        m_sVisProbLandDecrementOnUnseen = pc.visProbLandDecrementOnUnseen;
        m_sbVisTargetExactMatchOnly = pc.visTargetExactMatchOnly;
        m_sbStaticsInitialised = true;
    }

    // initialise this instance's visual preference data
    std::normal_distribution<float> dist(0.0, pc.visProbLandNonTargetIndivStdDev);

    if (m_ConstancyType == PollinatorConstancyType::VISUAL) {
        switch (ModelParams::getColourSystem())
        {
            case ColourSystem::REGULAR_MARKER_POINTS: {
                for (MarkerPoint mp = m_sVisDataMPMin; mp <= m_sVisDataMPMax; mp += m_sVisDataMPStep)
                {
                    float baseProbLandNonTargetInnate = getBaseProbLandNonTargetInnate(mp);
                    float baseProbLandNonTargetIndivDelta = dist(EvoBeeModel::m_sRngEngine);
                    float baseProbLandNonTarget = baseProbLandNonTargetInnate + baseProbLandNonTargetIndivDelta;
                    m_VisualPreferences.emplace_back(mp, m_sVisBaseProbLandTarget, baseProbLandNonTarget);
                }
                break;
            }
            case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS: {
                // Note that this code was implemented as an amendment to the constructor to deal with
                // the case of ColourSystem = ARBITRARY_DOMINANT_WAVELENGTHS. But actually we could just
                // as well remove the REGULAR_MARKER_POINTS case above and initialise m_VisualPreferences
                // as shown below in ALL cases. The end result should be the same.
                //
                // This way of doing it is actually better because it makes explicit a property that
                // we rely on elsewhere in the code (e.g. in the Hymenoptera::getVisXXXFromMP methods) -
                // that the m_VisualPrefences and m_sVisData vectors mirror each other in terms of
                // number and order of entries with respect to markerpoints/wavelengths (so the same index
                // refers to the same wavelength in both vectors).
                //
                // For the time being we have left the old implementation of the REGULAR_MARKER_POINTS
                // case above as is, on the basis of "if it ain't broke, don't fix it".
                for (auto& vsi : m_sVisData) {
                    Wavelength lambda = vsi.getWavelength();
                    float baseProbLandNonTargetInnate = getBaseProbLandNonTargetInnate(lambda);
                    float baseProbLandNonTargetIndivDelta = dist(EvoBeeModel::m_sRngEngine);
                    float baseProbLandNonTarget = baseProbLandNonTargetInnate + baseProbLandNonTargetIndivDelta;
                    m_VisualPreferences.emplace_back(&vsi, m_sVisBaseProbLandTarget, baseProbLandNonTarget);
                }
                break;
            }
            default: {
                throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera constructor. Aborting!");
            }
        }
    }

    if (m_LearningStrategy == PollinatorLearningStrategy::STAY_RND) {
        pickRandomTarget();
    }
    else if (m_LearningStrategy == PollinatorLearningStrategy::STAY_INNATE) {
        initialiseInnateTarget();
    }
}

Hymenoptera::Hymenoptera(const Hymenoptera& other) :
    Pollinator(other),
    m_VisualPreferences(other.m_VisualPreferences)
{
}

Hymenoptera::Hymenoptera(Hymenoptera&& other) noexcept :
    Pollinator(std::move(other)),
    m_VisualPreferences(std::move(other.m_VisualPreferences))
{
}

void Hymenoptera::reset()
{
    Pollinator::reset();

    for (VisualPreferenceInfo& vpi : m_VisualPreferences)
    {
        vpi.reset();
    }

    if (m_LearningStrategy == PollinatorLearningStrategy::STAY_RND) {
        pickRandomTarget();
    }
    else if (m_LearningStrategy == PollinatorLearningStrategy::STAY_INNATE) {
        initialiseInnateTarget();
    }
}

void Hymenoptera::pickRandomTarget()
{
    assert(m_sbStaticsInitialised);

    // this current implementation only works with ColourSystem::REGULAR_MARKER_POINTS...
    assert(ModelParams::getColourSystem() == ColourSystem::REGULAR_MARKER_POINTS);

    int numMPs = (int)((m_sVisDataMPMax - m_sVisDataMPMin) / m_sVisDataMPStep) + 1;
    if (numMPs <= 1) {
        setTargetWavelength(m_sVisDataMPMin);
    }
    else {
        std::uniform_int_distribution<int> dist(0, numMPs-1);
        int idx = dist(EvoBeeModel::m_sRngEngine);
        setTargetWavelength(m_sVisDataMPMin + idx * m_sVisDataMPStep);
    }
}


void Hymenoptera::initialiseInnateTarget()
{
    assert(m_sbStaticsInitialised);

    switch (ModelParams::getColourSystem()) {
        case ColourSystem::REGULAR_MARKER_POINTS: {
            initialiseInnateTargetRegular();
            break;
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS: {
            initialiseInnateTargetArbitrary();
            break;
        }
        default: {
            throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera::initialiseInnateTarget. Aborting!");
        }
    }

    if (ModelParams::verbose()) {
        std::cout << "T, " << getTargetWavelength() << std::endl;
    }
}


// When using regular marker points, we can initialise the target marker point directly from the cumulative normalised
// preference data from Giurfa. We just throw a dart at the cumulative figure and see where it lands, then take the
// closest step value below that as the selected marker point.
void Hymenoptera::initialiseInnateTargetRegular()
{
    assert(m_sVisDataMPMin == 300);
    assert(m_sVisDataMPMax == 650);
    assert(ModelParams::getColourSystem() == ColourSystem::REGULAR_MARKER_POINTS);

    MarkerPoint mp = 0;

    float selection = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine);

    for (auto it = m_sGiurfaCumulativeInnatePrefs.begin(); it != m_sGiurfaCumulativeInnatePrefs.end(); ++it) {
        if (std::get<2>(*it) >= selection) {
            mp = std::get<0>(*it);
            break;
        }
    }

    assert (mp >= 300);
    assert (mp <= 650);

    if (m_sVisDataMPStep > 1) {
        mp = (mp / m_sVisDataMPStep) * m_sVisDataMPStep; // round marker point to the nearest step below the chosen point
    }

    setTargetWavelength(mp);
}


// When using arbitrary dominant wavelengths, we pick a target wavelength by looking at the set of available
// dominant wavelengths (as defined in the PlantTypeConfigs), find the giurfa preference for each of those, normalise
// the list of found preferences, and use that as the basis from which to pick a dominant wavelength.
void Hymenoptera::initialiseInnateTargetArbitrary()
{
    // This method makes use of the following static map that is populated the first time we call the method.
    // It maps a dominant wavelength to a tuple comprising the following elements:
    // 0. (int) Number of PlantTypeConfigs that match this dominant wavelength
    // 1. (float) The Giurfa preference for this dominant wavelength
    // 2. (float) The raw (unnormalised) probability of picking this wavelength (PlambdaRaw)
    // 3. (float) The normalised probability of picking this wavelength (PlambdaNorm)
    // 4. (float) The cumulative sum of PlambdaNorms for all wavelengths up to and including this one
    // 5. (vector) A list of pointers to all the PlantTypeConfigs that have this wavelength
    static std::map<Wavelength, std::tuple<int, float, float, float, float, std::vector<const PlantTypeConfig*>>> prefData;
    static bool cumulativeInnatePrefsInitialised = false;

    if (!cumulativeInnatePrefsInitialised) {
        auto& ptcs = ModelParams::getPlantTypeConfigs();
        for (auto& ptc : ptcs) {
            Wavelength lambda = ptc.getDominantWavelength();
            auto it = prefData.find(lambda);
            if (it == prefData.end()) {
                std::vector<const PlantTypeConfig*> ptcvec {&ptc};
                float gpref = getGiurfaPref(lambda);
                prefData.insert(std::make_pair(lambda, std::make_tuple(1, gpref, 0.0f, 0.0f, 0.0f, ptcvec)));
            }
            else {
                std::get<0>(it->second)++;               // increment count of number of PTCs with this wavelength
                std::get<5>(it->second).push_back(&ptc); // add a pointer to this PTC to the vector of pointers
            }
        }

        float numPTCs = (float)(ptcs.size());
        float sumPlambdaRaw = 0.0;

        for (auto& entry : prefData) {
            // calculate and set PlambdaRaw for this wavelength
            float PlambdaRaw = (((float)std::get<0>(entry.second)) * std::get<1>(entry.second)) / numPTCs;
            std::get<2>(entry.second) = PlambdaRaw;
            // and update running count of sum of all PlambdaRaws
            sumPlambdaRaw += PlambdaRaw;
        }

        float cumPlambdaNorm = 0.0;
        for (auto& entry : prefData) {
            // calculate and set PlambdaNorm for this wavelength
            float PlambdaNorm = std::get<2>(entry.second) / sumPlambdaRaw;
            std::get<3>(entry.second) = PlambdaNorm;
            // and record the cumulative sum of all PlambdaNorms up to this point
            cumPlambdaNorm += PlambdaNorm;
            std::get<4>(entry.second) = cumPlambdaNorm;
        }

        cumulativeInnatePrefsInitialised = true;
    }


    // to assign a target we use roulette wheel selection, picking a number at random between [0,1) and selecting
    // the first wavelength whose cumulative
    float selection = EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine);

    // std::map is a sorted container, so we can be sure when we are traversing its members that we are
    // going sequentially from lowest to highest wavelength
    for (auto& entry : prefData) {
        if ((std::get<4>(entry.second) + EvoBee::FLOAT_COMPARISON_EPSILON) >= selection) {
            std::uniform_int_distribution<int> dist(0, std::get<5>(entry.second).size() - 1);
            const PlantTypeConfig* pPTC = std::get<5>(entry.second).at(dist(EvoBeeModel::m_sRngEngine));
            m_TargetReflectance.setVisDataPtr(pPTC->flowerVisDataPtr);
            return;
        }
    }

    // if we get to this point, it means that the final PlambdaNorm recorded in prefData is less than 1.0, so
    // something has gone horribly wrong in the calculations...
    std::stringstream msg;
    msg << "Unexpected error encountered in the calculations in Hymenoptera::initialiseInnateTargetArbitrary()\n";
    msg << " Content of prefData map:\n";
    for (auto& entry : prefData) {
        msg << "   " << entry.first << " -> (" << std::get<0>(entry.second) << ", " << std::get<1>(entry.second) << ", "
            << std::get<2>(entry.second) << ", " << std::get<3>(entry.second) << ", " 
            << std::get<4>(entry.second) << ")\n";
    }  
    msg << " Value of selection was " << selection << "\n"; 
    throw std::runtime_error(msg.str());
}


// return the normalised giurfa preference level associated with the given wavelength
float Hymenoptera::getGiurfaPref(MarkerPoint lambda)
{
    for (auto it = m_sGiurfaCumulativeInnatePrefs.begin(); it != m_sGiurfaCumulativeInnatePrefs.end(); ++it) {
        if (std::get<0>(*it) == lambda) {
            return std::get<1>(*it);
        }
    }

    std::stringstream msg;
    msg << "Error: unable to find matching preference level for wavelength " << lambda << " in Hymenoptera::getGiurfaPref()";
    throw std::runtime_error(msg.str());

    return 0.0f; // to keep the compiler happy
}


// Output the hymenoptera's current state as a string, suitable for logging purposes.
// Note, if you are looking at this code to understand the contents of a log file,
// remember that the Hymenoptera class is a virtual class. Classes that
// inherit from Hymenoptera may append additional information to this string,
// e.g. see HoneyBee::getStateString().
std::string Hymenoptera::getStateString() const
{
    std::string polState = Pollinator::getStateString();
    std::stringstream ssState;
    ssState << std::fixed << std::setprecision(3);
    ssState << ",//," << getTargetWavelength() << ",::,";
    for (auto& vpi : m_VisualPreferences)
    {
        ssState << vpi.getWavelength() << "," << vpi.probLandTarget << "," << vpi.probLandNonTarget << ",";
    }
    ssState << "::";
    return polState + ssState.str();
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
// that corresponds to the given Marker Point
std::size_t Hymenoptera::getVisualDataVectorIdx(MarkerPoint mp)
{
    assert(m_sbStaticsInitialised);

    std::size_t idx = 0;

    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
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

            idx = (std::size_t)((mp - m_sVisDataMPMin) / m_sVisDataMPStep);
            break;
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
        {
            assert(false);  // we probably don't want to be using this method for ARBITRARY_DOMINANT_WAVELENGTHS
                            // because there may be multiple entries in m_sVisData that match a given wavelength

            for (idx = 0; idx < m_sVisData.size(); ++idx)
            {
                if (m_sVisData[idx].getWavelength() == mp) {
                    break;
                }
            }
            if (idx >= m_sVisData.size()) {
                std::stringstream msg;
                msg << "Error in Hymenoptera::getVisualDataVectorIdx:"
                    << "  Wavelength " << mp << " not found in vis-data.\n";
                throw std::runtime_error(msg.str());
            }
            break;
        }
        default:
        {
            throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera::getVisualDataVectorIdx. Aborting!");
        }
    }

    return idx;
}


const VisualStimulusInfo& Hymenoptera::getVisStimInfoFromMP(MarkerPoint mp)
{
    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
            std::size_t idx = getVisualDataVectorIdx(mp);
            return m_sVisData.at(idx);
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS: {
            auto it = std::find_if( m_sVisData.begin(),
                                    m_sVisData.end(),
                                    [mp](VisualStimulusInfo& vsi){return (vsi.getWavelength() == mp);});
            if (it == m_sVisData.end()) {
                std::stringstream msg;
                msg << "Unable to find entry in m_sVisData for mp=" << mp << " in Hymenoptera::getVisStimInfoFromMP! Aborting.\n";
                throw std::runtime_error(msg.str());
            }
            return (*it);
        }
        default:
        {
            throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera::getVisStimInfoFromMP. Aborting!\n");
        }
    }
}

VisualPreferenceInfo& Hymenoptera::getVisPrefInfoFromWavelength(Wavelength lambda)
{
    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
            std::size_t idx = getVisualDataVectorIdx(lambda);
            return m_VisualPreferences.at(idx);
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
        {
            auto it = std::find_if( m_VisualPreferences.begin(),
                                    m_VisualPreferences.end(),
                                    [lambda](VisualPreferenceInfo& vpi){return (vpi.getWavelength() == lambda);});
            if (it == m_VisualPreferences.end()) {
                std::stringstream msg;
                msg << "Unable to find entry in m_VisualPreferences for wavelength=" << lambda << " in Hymenoptera::getVisPrefInfoFromWavelength! Aborting.\n";
                throw std::runtime_error(msg.str());
            }
            return (*it);
        }
        default:
        {
            throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera::getVisPrefInfoFromWavelength. Aborting!\n");
        }
    }
}

const VisualPreferenceInfo& Hymenoptera::getVisPrefInfoFromWavelengthConst(Wavelength lambda) const
{

    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
            std::size_t idx = getVisualDataVectorIdx(lambda);
            return m_VisualPreferences.at(idx);
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
        {
            auto it = std::find_if( m_VisualPreferences.begin(),
                                    m_VisualPreferences.end(),
                                    [lambda](const VisualPreferenceInfo& vpi){return (vpi.getWavelength() == lambda);});
            if (it == m_VisualPreferences.end()) {
                std::stringstream msg;
                msg << "Unable to find entry in m_VisualPreferences for wavelength=" << lambda << " in Hymenoptera::getVisPrefInfoFromWavelengthConst! Aborting.\n";
                throw std::runtime_error(msg.str());
            }
            return (*it);
        }
        default:
        {
            throw std::runtime_error("Encountered unexpected ColourSystem specification in Hymenoptera::getVisPrefInfoFromWavelengthConst. Aborting!\n");
        }
    }
}

float Hymenoptera::getBaseProbLandNonTargetInnate(MarkerPoint mp)
{
    return getVisStimInfoFromMP(mp).baseProbLandNonTargetInnate;
}


// Make a probabilistic decision, for a given marker point, on whether the pollinator
// can detect it.
bool Hymenoptera::isDetected(Wavelength lambda) const
{
    float detectionProb = 0.0;

    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:
        {
            detectionProb  = getMPDetectionProb(lambda);
            break;
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:
        {
            const VisualStimulusInfo* pVSI = m_TargetReflectance.getVisDataPtr();
            if (pVSI == nullptr) throw std::runtime_error("Encountered a null pointer in Hymenoptera::isDetected()");
            detectionProb = pVSI->detectionProb;
            break;
        }
        default:
        {
            throw std::runtime_error("Encountered an unknown ColourSystem in Hymenoptera::isDetected()");
        }
    }

    if (detectionProb > (1.0f - EvoBee::FLOAT_COMPARISON_EPSILON)) {
        return true;
    }
    else if (detectionProb < (0.0f + EvoBee::FLOAT_COMPARISON_EPSILON)) {
        return false;
    }
    else {
        return (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < detectionProb);
    }
}


// Determine whether the pollinator will decide to land on the given flower,
// using its visual perception. This entails three separate steps, (1) DETECT
// the flower, (2) DISTINGUISH  whether or not the flower matches the pollinator's
// current target, and (3) DECIDE whether or not to land based upon the pollinator's
// learned probabilities of landing on a target or non-target flower as appropriate.
//
// This method performs the second and third of these steps (DISTINGUISH and DECIDE).
// The first step was already dealt with in Pollinator::forageNearestFlower.
//
// The second parameter, pJudgedToMatchTarget, is an optional pointer to a boolean
// variable used to return an indication of whether or not the pollinator judged
// the flower to match its target (whether or not it landed on it).
///
bool Hymenoptera::isVisitCandidateVisual(Flower* pFlower, bool* pJudgedToMatchTarget) const
{
    bool bIsVisitCandidate = false;
    bool bNoTargetSet = (getTargetWavelength() == NO_MARKER_POINT);
    bool bJudgedToBeTarget = false;

    // (1) DETECT step
    // We have already dealt with the pollinator's DETECT step by calling Pollinator::isDetected in
    // Environment::findNearestUnvisitedFlower called from Pollinator::forageNearestFlower

    // (2) DISTINGUISH step
    if (!bNoTargetSet)
    {
        if (m_sbVisTargetExactMatchOnly)
        {
            bJudgedToBeTarget = (getTargetWavelength() == pFlower->getCharacteristicWavelength());
        }
        else
        {
            float confidenceOfMatch = confidenceMatchesTarget(pFlower->getReflectanceInfo());
            bJudgedToBeTarget = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < confidenceOfMatch);
        }
        if (pJudgedToMatchTarget != nullptr) {
            *pJudgedToMatchTarget = bJudgedToBeTarget;
        }
    }

    // (3) DECIDE step
    // Here we make use of the pollinator's learned probabilities of landing on a target or non-target flower
    // to make the final decision of whether to land
    Wavelength flowerLambda = pFlower->getCharacteristicWavelength();
    const VisualPreferenceInfo& visPrefInfo = getVisPrefInfoFromWavelengthConst(flowerLambda);

    if (bNoTargetSet)
    {
        float landProb = visPrefInfo.getProbLandNonTarget() + m_sVisProbLandNoTargetSetDelta;
        bIsVisitCandidate = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < landProb);
    }
    else
    {
        float landProb = bJudgedToBeTarget ? visPrefInfo.getProbLandTarget() : visPrefInfo.getProbLandNonTarget();
        bIsVisitCandidate = (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < landProb);
    }

    return bIsVisitCandidate;
}


// Update the pollinator's visual preference info after each visit to a flower.
void Hymenoptera::updateVisualPreferences(const Flower* pFlower, int nectarCollected)
{
    switch (m_LearningStrategy) {
        case PollinatorLearningStrategy::FICKLE_CIRCUMSPECT: {
            updateVisualPrefsFickleCircumspect(pFlower, nectarCollected);
            break;
        }
        case PollinatorLearningStrategy::STAY: {
            updateVisualPrefsStay(pFlower, nectarCollected);
            break;
        }
        case PollinatorLearningStrategy::STAY_RND: {
            // do nothing
            break;
        }
        case PollinatorLearningStrategy::STAY_INNATE: {
            // do nothing
            break;
        }
        case PollinatorLearningStrategy::NONE: {
            // do nothing
            break;
        }
        case PollinatorLearningStrategy::DELIBERATIVE_DECISIVE: // NOT YET IMPLEMENTED
        default: {
            throw std::runtime_error("Unhandled learning strategy encountered in Hymenoptera::updateVisualPreferences");
        }
    }
}


// Update the pollinator's visual preference info according to the fickle-circumspect strategy
void Hymenoptera::updateVisualPrefsFickleCircumspect(const Flower* pFlower, int nectarCollected)
{
    const ReflectanceInfo& flowerReflectance = pFlower->getReflectanceInfo();
    Wavelength flowerLambda = flowerReflectance.getCharacteristicWavelength();
    VisualPreferenceInfo& visPrefInfo = getVisPrefInfoFromWavelength(flowerLambda);
    bool isTarget = (flowerLambda == getTargetWavelength());
    bool noTarget = (getTargetWavelength() == NO_MARKER_POINT);
    bool firstTarget = false;

    if (nectarCollected > 0)
    {
        // if the visitied flower was rewarding...
        if (noTarget) {
            setTargetWavelength(flowerReflectance.getVisDataPtr());
            visPrefInfo.setAsTarget();
            firstTarget = true;
            noTarget = false;
        }
        else if (isTarget) {
            visPrefInfo.incrementProbLandTarget(m_sVisProbLandIncrementOnReward);
        }
        else {
            visPrefInfo.incrementProbLandNonTarget(m_sVisProbLandIncrementOnReward);
        }
    }
    else
    {
        // if the visited flower was not rewarding...
        if (isTarget) {
            visPrefInfo.decrementProbLandTarget(m_sVisProbLandDecrementOnNoReward);
        }
        else {
            visPrefInfo.decrementProbLandNonTarget(m_sVisProbLandDecrementOnNoReward);
        }
    }

    // consider switching to a new target flower based upon the newly updated preferences
    if ((!firstTarget) && (!noTarget)) {
        updateTarget();
    }

    // attenuate preferences for non-target flowers that have not been recently encountered
    attenuatePreferences();
}


// Update the pollinator's visual preference info according to the stay strategy
void Hymenoptera::updateVisualPrefsStay(const Flower* pFlower, int nectarCollected)
{
    if ((getTargetWavelength() == NO_MARKER_POINT) && (nectarCollected > 0))
    {
        const ReflectanceInfo& flowerReflectance = pFlower->getReflectanceInfo();
        Wavelength flowerLambda = flowerReflectance.getCharacteristicWavelength();
        VisualPreferenceInfo& visPrefInfo = getVisPrefInfoFromWavelength(flowerLambda);
        setTargetWavelength(flowerLambda);
        visPrefInfo.setAsTarget();
    }
}


// Consider switching to a new target flower colour based upon newly updated preferences
void Hymenoptera::updateTarget()
{
    // this implementation only works with ColourSystem::REGULAR_MARKER_POINTS at this stage
    // because the call to setTargetWavelength only takes a Wavelength at present - it really
    // needs to take a ptr to a VisStimInfo object - so VisPrefInfo should really store one
    // of those instead of a plain wavelength... TODO
    assert(ModelParams::getColourSystem() == ColourSystem::REGULAR_MARKER_POINTS);

    float maxProbLandNonTarget = 0.0;
    VisualPreferenceInfo& vpiCurrentTarget = getVisPrefInfoFromWavelength(getTargetWavelength());
    VisualPreferenceInfo* pVpiMaxNonTarget = nullptr;

    // find the non-target marker point that currently has the highest landing probability
    for (auto& vpi : m_VisualPreferences)
    {
        if ((vpi.getWavelength() != getTargetWavelength()) && (vpi.getProbLandNonTarget() > maxProbLandNonTarget))
        {
            pVpiMaxNonTarget = &vpi;
            maxProbLandNonTarget = vpi.getProbLandNonTarget();
        }
    }

    // if the landing probability of the highest non-target marker point is higher than that of the
    // current target marker point, make it the new target
    if ((pVpiMaxNonTarget != nullptr) && (maxProbLandNonTarget > vpiCurrentTarget.getProbLandTarget()))
    {
        setTargetWavelength(pVpiMaxNonTarget->getWavelength());
                                            // make the (old) non-target flower the new target

        pVpiMaxNonTarget->setAsTarget();    // set the (old) non-target flower's target prob land to the default target prob land

        vpiCurrentTarget.setAsNonTarget();  // set the (old) target flower's non-target prob land to the (old) target flower's target prob land
                                            // (i.e. it used to have a high preference, keep it, but now use it as the non-target preference)
    }
}


// Attenuate preferences for non-target flowers that have not been recently visited
void Hymenoptera::attenuatePreferences()
{
    for (auto& vpi : m_VisualPreferences)
    {
        Wavelength lambda = vpi.getWavelength();
        if (std::find_if(m_RecentlyVisitedFlowers.begin(),
                         m_RecentlyVisitedFlowers.end(),
                         [lambda](Flower* pRecentFlower){return (pRecentFlower->getCharacteristicWavelength() == lambda);})
            == m_RecentlyVisitedFlowers.end())
        {
            // This marker point has not been recently seen
            vpi.decrementProbLandNonTarget(m_sVisProbLandDecrementOnUnseen);// decrement its non-target prob land
                                                                            // (i.e. forget the preference for it a little)
        }
    }
}
