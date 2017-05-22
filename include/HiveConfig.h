/**
 * @file
 *
 * Declaration of the HiveConfig class
 */

#ifndef _HIVECONFIG_H
#define _HIVECONFIG_H

#include <string>

/**
 * The HiveConfig class ...
 */
struct HiveConfig {
    std::string type;
    int num;
    bool startFromHive;
    float x;
    float y;
};

#endif /* _HIVECONFIG_H */
