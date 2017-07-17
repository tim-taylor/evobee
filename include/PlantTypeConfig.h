/**
 * @file
 *
 * Declaration of the PlantTypeConfig class
 */

#ifndef _PLANTTYPECONFIG_H
#define _PLANTTYPECONFIG_H

#include <string>
#include "ReflectanceInfo.h"

/**
 * The PlantTypeConfig class ...
 */
struct PlantTypeConfig {
    PlantTypeConfig() :
        pollenClogging(false),
        numFlowers(1),
        hasLeaf(false)
    {};

    std::string species;
    MarkerPoint flowerMPInitMin;
    MarkerPoint flowerMPInitMax;
    int         antherInitPollen;
    int         antherPollenTransferPerVisit;
    int         stigmaMaxPollenCapacity;
    bool        pollenClogging;
    //int       nectarReward; // (currently unused)
    float       initTemp; ///<@todo should be initialised // init temp of flowers and leaves
    int         numFlowers;
    bool        hasLeaf;
    MarkerPoint leafMP;
};

#endif /* _PLANTTYPECONFIG_H */
