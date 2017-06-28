/**
 * @file
 *
 * Declaration of the HiveConfig class
 */

#ifndef _HIVECONFIG_H
#define _HIVECONFIG_H

#include <string>
#include "Position.h"


/**
 * The HiveConfig class ...
 */
struct HiveConfig {
    std::string type;               ///< Pollinator type
    int     num;                    ///< Number of pollinators
    bool    startFromHive;          ///< Do all pollinators start at hive, or from random pos
                                    ///<   in initial foraging area?
    fPos    position;               ///< Position of the hive
    iPos    areaTopLeft;            ///< Top-left corner of initial foraging area
    iPos    areaBottomRight;        ///< Bottom-right corner of initial foraging area
    bool    migrationAllowed;       ///< Are pollinators allowed to move outside initial foraging area?
    bool    migrationRestricted;    ///< If migration allowed, is its probability restricted at all?
    float   migrationProb;          ///< If migration allowed and restricted, and pollinator tries to
                                    ///<   leave initial foraging area, it succeeds with this prob,
                                    ///<   else it is reflected back into initial area
};

#endif /* _HIVECONFIG_H */
