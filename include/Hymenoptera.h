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

    /**
     * Overridden implementation of a method to make a probabilistic decision,
     * for a given characteristic wavelength, on whether the pollinator can detect it.
     */
    bool isDetected(Wavelength lambda) const override;

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

    /**
     * Returns information about the pollinator's current preferences for
     * a stimulus with the specified characteristic wavelength
     */
    const VisualPreferenceInfo& getVisPrefInfoFromWavelengthConst(Wavelength lambda) const;
    VisualPreferenceInfo&       getVisPrefInfoFromWavelength(Wavelength lambda);

    /**
     * Overridden implementation of method to determine whether the pollinator should
     * harvest the specified flower using its visual perception. This is a special case
     * called by isVisitCandidate() in the case that m_ConstancyType is VISUAL.
     */
    bool isVisitCandidateVisual(Flower* pFlower, bool* pJudgedToMatchTarget = nullptr) const override;

    /**
     * Overridden implementation of method to update the pollinator's visual preference
     * data after each visit to a flower.
     */
    void updateVisualPreferences(const Flower* pFlower, int nectarCollected) override;


private:
    std::vector<VisualPreferenceInfo>       m_VisualPreferences;///< record of the pollinator's current preferences for
                                                                ///< difference visual stimuli

    static std::vector<VisualStimulusInfo>  m_sVisData;         ///< vector containing data relating to pollinator's
                                                                ///< visual sensation of stimuli at different
                                                                ///< marker point values indexed as with m_VisMPDetectionProbs

    static MarkerPoint              m_sVisDataMPMin;	        ///< defines the marker point of the first entry in the
                                                                ///< m_VisData vector

    static MarkerPoint              m_sVisDataMPStep;	        ///< defines the marker point increment between entries in the
                                                                ///< m_VisData vector

    static MarkerPoint              m_sVisDataMPMax;	        ///< defines the marker point of the final entry in the
                                                                ///< m_VisData vector

    static float                    m_sVisBaseProbLandTarget;   ///< the base probability that a pollinator of this species will
                                                                ///< land on a flower that it judges to match its target type

    static float                    m_sVisProbLandNoTargetSetDelta;     ///< the delta probability to add to the
                                                                        ///< baseProbLandNonTarget[mp] probability in the case
                                                                        ///< where the pollinator does not yet have any target

    static float                    m_sVisProbLandIncrementOnReward;    ///< learning rate: increase in landing probability when
                                                                        ///< the pollinator lands on a rewarding flower

    static float                    m_sVisProbLandDecrementOnNoReward;  ///< learning rate: decrease in landing probability when
                                                                        ///< the pollinator lands on a non-rewarding flower

    static float                    m_sVisProbLandDecrementOnUnseen;    ///< learning rate: decrease in landing probability when
                                                                        ///< pollinator hasn't seen a particular marker point recently

    static bool                     m_sbVisTargetExactMatchOnly;///< Overrides the normal visual discrimination code and will only
                                                                ///< match a stimulus with the target if they share the exact same
                                                                ///< marker point

    static std::vector<std::tuple<MarkerPoint, float, float>> m_sGiurfaCumulativeInnatePrefs; ///< Stores information about innate
                                                                ///< colour preferences for hymenoptera as described in Fig 3 of
                                                                ///< Giurfa et al, J. Comp. Physiol A (1995).

    static std::vector<std::tuple<MarkerPoint, float, float>> m_sHoverflyCumulativeInnatePrefs; ///< Stores information about innate
                                                                ///< colour preferences for hoverflies as described in Fig 5 of
                                                                ///< Lunau & Wacht, J. Comp. Physiol A (1994).

    static bool                     m_sbStaticsInitialised;     ///< Flags whether statics have been initialised from config file

    /**
     * String description of the Hymenoptera type, for use in log files
     */
    static std::string m_sTypeNameStr;

    // private helper methods
    void updateTarget();
    void attenuatePreferences();
    void updateVisualPrefsFickleCircumspect(const Flower* pFlower, int nectarCollected);
    void updateVisualPrefsStay(const Flower* pFlower, int nectarCollected);
    void pickRandomTarget();
    void initialiseInnateTarget();
    void initialiseInnateTargetRegular();
    void initialiseInnateTargetArbitrary();
    float getInnatePref(Wavelength lambda);
    float getGiurfaPref(Wavelength lambda);
    float getHoverflyPref(Wavelength lambda);

    static const VisualStimulusInfo& getVisStimInfoFromMP(MarkerPoint mp);
    static std::size_t getVisualDataVectorIdx(MarkerPoint mp);
    static float getBaseProbLandNonTargetInnate(MarkerPoint mp);
};

#endif /* _HYMENOPTERA_H */
