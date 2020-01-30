/**
 * @file
 *
 * Declaration of the PlantTypeDistributionConfig class
 */

#ifndef _PLANTTYPEDISTRIBUTIONCONFIG_H
#define _PLANTTYPEDISTRIBUTIONCONFIG_H

#include <string>
#include "Position.h"

/**
 * The PlantTypeDistributionConfig class ...
 */
struct PlantTypeDistributionConfig {

    PlantTypeDistributionConfig() :
        id(0),
        species("Unknown"),
        density(0.1),
        refuge(false),
        refugeAlienInflowProb(0.0),
        seedOutflowAllowed(true),
        seedOutflowRestricted(false),
        seedOutflowProb(0.5),
        reproLocalDensityConstrained(false),
        reproLocalDensityMax(0.5)
    {}

    unsigned int id;                ///< Each PTDC has a unique id assigned to it
                                    ///<   (assignment is done in ModelParams::addPlantTypeDistributionConfig)

    std::string species;            ///< Name of the plant species
    iPos    areaTopLeft;            ///< Top-left corner of initial distribution area
    iPos    areaBottomRight;        ///< Bottom-right corner of initial distribution area
    float   density;                ///< Density of initial distribution (wrt specified distrib area)
    bool    refuge;                 ///< Is this area a refuge for this plant species?
    float   refugeAlienInflowProb;  ///< If this is a refuge, what is the probability that an
                                    ///< alien species attempting to germinate in this area will
                                    ///< do so successfully
    bool    seedOutflowAllowed;     ///< Can the plants germinate outside of the initial distrib area?
    bool    seedOutflowRestricted;  ///< If seedOutflowAllowed, is it restricted?
    float   seedOutflowProb;        ///< If seedOutflowAllowed and seedOutflowRestricted, what is
                                    ///<    the probability that they are successful if attempting
                                    ///<    to do so?
    bool    reproLocalDensityConstrained;   ///< During reproduction, is the maximum density of
                                            ///<    plants in this local area constrained?
    float   reproLocalDensityMax;   ///< If reproLocalDensityConstrained, this is the maximum
                                    ///<    allowed density
};

#endif /* _PLANTTYPEDISTRIBUTIONCONFIG_H */
