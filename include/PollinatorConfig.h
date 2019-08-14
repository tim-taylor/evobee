/**
 * @file
 *
 * Declaration of the PollinatorConfig class
 */

#ifndef _POLLINATORCONFIG_H
#define _POLLINATORCONFIG_H

#include <string>
#include "PollinatorEnums.h"
#include "ReflectanceInfo.h"

/**
 * The PollinatorConfig class ...
 */
struct PollinatorConfig {
    PollinatorConfig() {};

    std::string species;
    int boutLength;
    int maxPollenCapacity;
    int pollenDepositPerFlowerVisit;
    int pollenLossInAir;
    int pollenCarryoverNumVisits;
    MarkerPoint innateMPPrefMin;
    MarkerPoint innateMPPrefMax;
    std::string strConstancyType;          ///< allowed values: none, simple
    PollinatorConstancyType constancyType;
    float constancyParam;
    std::string strForagingStrategy;       ///< allowed values: random, nearest-flower, random-flower, random-global
    PollinatorForagingStrategy foragingStrategy;
    unsigned int visitedFlowerMemorySize;
};

#endif /* _POLLINATORCONFIG_H */
