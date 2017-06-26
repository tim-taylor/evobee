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
        density(0.1),
        refuge(false),
        seedOutflowAllowed(true)
    {}

    std::string species;            ///< Name of the plant species
    iPos    areaTopLeft;            ///< Top-left corner of initial distribution area
    iPos    areaBottomRight;        ///< Bottom-right corner of initial distribution area
    float   density;                ///< Density of initial distribution (wrt specified distribution area)
    bool    refuge;                 ///< Is this area a refuge for this plant species?
    float   refugeAlienInflowProb;  ///< If this is a refuge, what is the probability that an 
                                    ///< alien species attempting to germinate in this area will
                                    ///< do so successfully?
    bool    seedOutflowAllowed;     ///< Can the plants germinate outside of the initial distribution area?
    float   seedOutflowProb;        ///< If seedOutflowAllowed, what is the probability that they do so?
};

#endif /* _PLANTTYPEDISTRIBUTIONCONFIG_H */
