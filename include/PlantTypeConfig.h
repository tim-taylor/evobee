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
        pollenCloggingAll(false),
        pollenCloggingPartial(false),
        numFlowers(1),
        hasLeaf(false),
        reproSeedDispersalGlobal(false),
        reproSeedDispersalRadius(1.0),
        diffMPIsDiffSpecies(false)
    {};

    std::string species;
    MarkerPoint flowerMPInitMin;
    MarkerPoint flowerMPInitMax;
    int         antherInitPollen;
    int         antherPollenTransferPerVisit;
    int         stigmaMaxPollenCapacity;
    bool        pollenCloggingAll;
    bool        pollenCloggingPartial;
    std::string pollenCloggingSpecies;
    int         initNectar;
    float       initTemp;                   ///<@todo (unused just now) // init temp of flowers and leaves
    int         numFlowers;
    bool        hasLeaf;
    MarkerPoint leafMP;
    bool        reproSeedDispersalGlobal;   ///< can seeds be dispersed at random across whole environment?
    float       reproSeedDispersalRadius;   ///< expressed in env units (1.0=one patch), overridden
                                            ///<   if reproSeedDispersalGlobal=true
    bool        diffMPIsDiffSpecies;        ///< if true and flowerMPInitMin != flowerMPInitMax, plants
                                            ///<   created with different marker points are treated as
                                            ///<   different species (default=false)
};

#endif /* _PLANTTYPECONFIG_H */
