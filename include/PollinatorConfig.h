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
    PollinatorConfig() : visDataDefined(false) {};

    std::string species;
    int boutLength;
    int maxPollenCapacity;
    int pollenDepositPerFlowerVisit;
    int pollenLossInAir;
    int pollenCarryoverNumVisits;
    int nectarCollectPerFlowerVisit;
    MarkerPoint innateMPPrefMin;
    MarkerPoint innateMPPrefMax;
    std::string strConstancyType;          ///< allowed values: none, simple, visual
    PollinatorConstancyType constancyType;
    float constancyParam;
    std::string strForagingStrategy;       ///< allowed values: random, nearest-flower, random-flower, random-global
    PollinatorForagingStrategy foragingStrategy;
    unsigned int visitedFlowerMemorySize;

    bool visDataDefined;
	std::vector<VisualStimulusInfo> visData;
	MarkerPoint visDataMPMin;
	MarkerPoint visDataMPStep;
	MarkerPoint visDataMPMax;

    float visBaseProbLandTarget;
    float visProbLandNoTargetSetDelta;
    float visProbLandNonTargetIndivStdDev;
    float visProbLandIncrementOnReward;
    float visProbLandDecrementOnNoReward;
    float visProbLandDecrementOnUnseen;
};

#endif /* _POLLINATORCONFIG_H */
