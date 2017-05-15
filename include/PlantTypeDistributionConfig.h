/**
 * @file
 *
 * Declaration of the PlantTypeDistributionConfig class
 */

#ifndef _PLANTTYPEDISTRIBUTIONCONFIG_H
#define _PLANTTYPEDISTRIBUTIONCONFIG_H

#include <string>

/**
 * The PlantTypeDistributionConfig class ...
 */
struct PlantTypeDistributionConfig {
    std::string plantType;
    std::string distribution;
    float density;
};

#endif /* _PLANTTYPEDISTRIBUTIONCONFIG_H */
