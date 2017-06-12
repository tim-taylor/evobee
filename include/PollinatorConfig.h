/**
 * @file
 *
 * Declaration of the PollinatorConfig class
 */

#ifndef _POLLINATORCONFIG_H
#define _POLLINATORCONFIG_H

#include <string>


/**
 * The PollinatorConfig class ...
 */
struct PollinatorConfig {
    PollinatorConfig() {};

    std::string species;
    int boutLength;
    int maxPollenCapacity;
    int pollenLossOnFlower;
    int pollenLossInAir;
    int pollenCarryoverNumVisits;
};

#endif /* _POLLINATORCONFIG_H */
