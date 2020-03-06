/**
 * @file
 *
 * Declaration of the PollinatorConfig class
 */

#ifndef _POLLINATORCONFIG_H
#define _POLLINATORCONFIG_H

#include <string>
#include <vector>
#include "PollinatorEnums.h"
#include "PollinatorStructs.h"
#include "ReflectanceInfo.h"

/**
 * The PollinatorConfig class ...
 */
struct PollinatorConfig {
    PollinatorConfig() :
        species("Unknown"),
        boutLength(100),
        maxPollenCapacity(0),
        pollenDepositPerFlowerVisit(3),
        pollenLossInAir(0),
        pollenCarryoverNumVisits(),
        nectarCollectPerFlowerVisit(100),
        visitedFlowerMemorySize(5),
        visDataDefined(false),
        visBaseProbLandTarget(0.9),
        visProbLandNoTargetSetDelta(0.2),
        visProbLandNonTargetIndivStdDev(0.01),
        visProbLandIncrementOnReward(0.01),
        visProbLandDecrementOnNoReward(0.01),
        visProbLandDecrementOnUnseen(0.005)
    {};

    std::string species;
    int boutLength;
    int maxPollenCapacity;
    int pollenDepositPerFlowerVisit;
    int pollenLossInAir;
    int pollenCarryoverNumVisits;
    int nectarCollectPerFlowerVisit;
    std::string strConstancyType;           ///< allowed values: none, simple, visual
    PollinatorConstancyType constancyType;
    float constancyParam;
    std::string strForagingStrategy;        ///< allowed values: random, nearest-flower, random-flower, random-global
    PollinatorForagingStrategy foragingStrategy;
    std::string strLearningStrategy;        ///< allowed values: stay, fickle-circumspect, deliberative-decisive
    PollinatorLearningStrategy learningStrategy;
    unsigned int visitedFlowerMemorySize;

    bool visDataDefined;                    ///< This is set to true if appropriate in extractVidDataFromPollinatorConfig in evobee.cpp.
	std::vector<VisualStimulusInfo> visData;///< Stores the processed data from the vis-data array. See extractVidDataFromPollinatorConfig in evobee.cpp.
	MarkerPoint visDataMPMin;               ///< This value is derived from the vis-data array. See extractVidDataFromPollinatorConfig in evobee.cpp.
	MarkerPoint visDataMPStep;              ///< This value is derived from the vis-data array. See extractVidDataFromPollinatorConfig in evobee.cpp.
	MarkerPoint visDataMPMax;               ///< This value is derived from the vis-data array. See extractVidDataFromPollinatorConfig in evobee.cpp.

    float visBaseProbLandTarget;            ///< Pollinator's base probability of landing on a target flower
    float visProbLandNoTargetSetDelta;      ///< If pollinator currently has no target, its innate probability of landing on a given marker point is increased by this amount.
    float visProbLandNonTargetIndivStdDev;  ///< Each pollinator's innate probability of landing on a given marker point has an element of stochasticity as determined by this parameter.
    float visProbLandIncrementOnReward;     ///< Learning algorithm parameter for increase in probability of landing on a marker point if the current flower is rewarding
    float visProbLandDecrementOnNoReward;   ///< Learning algorithm parameter for decrease in probability of landing on a marker point if the current flower is not rewarding
    float visProbLandDecrementOnUnseen;     ///< Learning algorithm parameter for decrease in probability of landing on a marker point if no flower with the marker point is currently in the Pollinator's m_RecentlyVisitedFlowers list.
};

#endif /* _POLLINATORCONFIG_H */
