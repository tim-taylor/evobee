/**
 * @file
 *
 * Declaration of the PlantTypeConfig class
 */

#ifndef _PLANTTYPECONFIG_H
#define _PLANTTYPECONFIG_H

#include <string>
#include <stdexcept>
#include "ReflectanceInfo.h"

struct VisualStimulusInfo;

/**
 * The PlantTypeConfig class ...
 */
struct PlantTypeConfig {
    PlantTypeConfig() :
        species("Unknown"),
        flowerMPInitMin(0),
        flowerMPInitMax(0),
        flowerMPInitStep(10),
        flowerVisDataID(-1),
        flowerVisDataPtr(nullptr),
        antherInitPollen(100),
        antherPollenTransferPerVisit(10),
        stigmaMaxPollenCapacity(5),
        pollenCloggingAll(false),
        pollenCloggingPartial(false),
        initNectar(100),
        numFlowers(1),
        hasLeaf(false),
        reproSeedDispersalGlobal(false),
        reproSeedDispersalRadiusStdDev(1.0),
        diffMPIsDiffSpecies(false)
    {};

    Wavelength getDominantWavelength() const;

    std::string species;
    MarkerPoint flowerMPInitMin;            ///< This is only used for ColourSystem==REGULAR_MARKER_POINTS
    MarkerPoint flowerMPInitMax;            ///< This is only used for ColourSystem==REGULAR_MARKER_POINTS
    MarkerPoint flowerMPInitStep;           ///< This is only used for ColourSystem==REGULAR_MARKER_POINTS
    int         flowerVisDataID;            ///< This is an id of a vis-data entry as specified in the config file
                                            ///<   (only used for ColourSystem==ARBITRARY_DOMINANT_WAVELENGTHS)
    const VisualStimulusInfo* flowerVisDataPtr; ///< In postprocessing after reading in the config file, flowerVisDataID
                                                ///<   is used to find a pointer to the identified entry in vis-data, which is
                                                ///<   recorded here (only used for ColourSystem==REGULAR_MARKER_POINTS)
    int         antherInitPollen;
    int         antherPollenTransferPerVisit;
    int         stigmaMaxPollenCapacity;
    bool        pollenCloggingAll;
    bool        pollenCloggingPartial;
    std::string pollenCloggingSpecies;      ///< Allowed values: empty string (does not clog any other species),
                                            ///<   "all" (clogs all other species), or a comma separated list
                                            ///<   of the names of the plant species that this species clogs
    int         initNectar;
    float       initTemp;                   ///<@todo (unused just now) // init temp of flowers and leaves
    int         numFlowers;
    bool        hasLeaf;
    MarkerPoint leafMP;
    bool        reproSeedDispersalGlobal;   ///< can seeds be dispersed at random across whole environment?
    float       reproSeedDispersalRadiusStdDev; ///< expressed in env units (1.0=one patch), ignored
                                                ///<   if reproSeedDispersalGlobal=true
    bool        diffMPIsDiffSpecies;        ///< if true and flowerMPInitMin != flowerMPInitMax, plants
                                            ///<   created with different marker points are treated as
                                            ///<   different species (default=false)
};

#endif /* _PLANTTYPECONFIG_H */
