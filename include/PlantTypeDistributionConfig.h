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
    int areaTopLeftX;
    int areaTopLeftY;
    int areaBottomRightX;
    int areaBottomRightY;
    float density;
};

#endif /* _PLANTTYPEDISTRIBUTIONCONFIG_H */
