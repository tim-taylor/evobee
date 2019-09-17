/**
 * @file
 *
 * Declaration of the Hymenoptera class
 */

#ifndef _HYMENOPTERA_H
#define _HYMENOPTERA_H

#include <string>
#include "PollinatorConfig.h"
#include "Pollinator.h"


/**
 * The Hymenoptera class ...
 */
class Hymenoptera : public Pollinator {

public:
    Hymenoptera(const PollinatorConfig& pc, AbstractHive* pHive);
    ~Hymenoptera() {}

    Hymenoptera(const Hymenoptera& other);
    Hymenoptera(Hymenoptera&& other) noexcept;

    void reset() override;

    //void step() override = 0;

    std::string getStateString() const override;

    const std::string& getTypeName() const override;

protected:
    /**
     * Returns the probability that this pollinator detects a visual stimulus which
     * possesses a single marker point as specified by the mp parameter.
     *
     * @return Detection probablility in the range 0.0 to 1.0
     */
    float getMPDetectionProb(MarkerPoint mp) const override;

    /**
     * Returns information about how a given stimulus with a single marker point
     * as specified by the mp parameter is perceived by this pollinator's visual
     * system.
     *
     * @return A const reference to a VisHexInfo which specifies the stimulus's
     * location in hexagonal colour space, its green contrast, and detection probability
     */
    const VisualStimulusInfo& getVisStimulusInfo(MarkerPoint mp) const override;

private:
    /*
    innate colour pref
    learned colour pref
    */

    static std::vector<VisualStimulusInfo>  m_sVisData;     ///< vector containing data relating to pollinator's
                                                            ///< visual sensation of stimuli at different
                                                            ///< marker point values indexed as with m_VisMPDetectionProbs

    static MarkerPoint              m_sVisDataMPMin;	    ///< defines the marker point of the first entry in the
                                                            ///< m_VisData vector

    static MarkerPoint              m_sVisDataMPStep;	    ///< defines the marker point increment between entries in the
                                                            ///< m_VisData vector

    static MarkerPoint              m_sVisDataMPMax;	    ///< defines the marker point of the final entry in the
                                                            ///< m_VisData vector

    static bool                     m_sbStaticsInitialised; ///< Flags whether statics have been initialised from config file

    /**
     * String description of the Hymenoptera type, for use in log files
     */
    static std::string m_sTypeNameStr;

    // private helper methods

    static const VisualStimulusInfo& getVisStimInfoFromMP(MarkerPoint mp);
};

#endif /* _HYMENOPTERA_H */
