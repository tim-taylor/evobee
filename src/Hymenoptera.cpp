/**
 * @file
 *
 * Implementation of the Hymenoptera class
 */

#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "Hymenoptera.h"
#include "PollinatorStructs.h"


// Instantiate static data members
std::string Hymenoptera::m_sTypeNameStr{"HYM"};
std::vector<VisualStimulusInfo> Hymenoptera::m_sVisData;
MarkerPoint Hymenoptera::m_sVisDataMPMin = 1;
MarkerPoint Hymenoptera::m_sVisDataMPStep = 1;
MarkerPoint Hymenoptera::m_sVisDataMPMax = 1;
bool Hymenoptera::m_sbStaticsInitialised = false;


Hymenoptera::Hymenoptera(const PollinatorConfig& pc, AbstractHive* pHive) :
    Pollinator(pc, pHive)
{
    if (!m_sbStaticsInitialised && pc.visDataDefined) {
        m_sVisDataMPMin = pc.visDataMPMin;
        m_sVisDataMPStep = pc.visDataMPStep;
        m_sVisDataMPMax = pc.visDataMPMax;
        m_sVisData = pc.visData;
        m_sbStaticsInitialised = true;
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

const VisualStimulusInfo& Hymenoptera::getVisStimInfoFromMP(MarkerPoint mp)
{
    assert(m_sbStaticsInitialised);

    if (mp < m_sVisDataMPMin)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisStimInfoFromMP:" << std::endl
            << "  Marker point " << mp << " below minimum value defined in config file (" << m_sVisDataMPMin << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    if ((mp - m_sVisDataMPMin) % m_sVisDataMPStep != 0)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisStimInfoFromMP:" << std::endl
            << "  Marker point " << mp << " is not at expected step size (" << m_sVisDataMPStep
            << ") above minimum value (" << m_sVisDataMPMin << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    if (mp > m_sVisDataMPMax)
    {
        std::stringstream msg;
        msg << "Error in Hymenoptera::getVisStimInfoFromMP:" << std::endl
            << "  Marker point " << mp << " is above maximum value defined in config file (" << m_sVisDataMPMax << ")" << std::endl;
        throw std::runtime_error(msg.str());
    }

    std::vector<VisualStimulusInfo>::size_type idx = (mp - m_sVisDataMPMin) / m_sVisDataMPStep;

    return m_sVisData.at(idx);
}