/**
 * @file
 *
 * Declaration of the Hive class
 */

#ifndef _ABSTRACTHIVE_H
#define _ABSTRACTHIVE_H

#include "Pollinator.h"
#include "HiveConfig.h"


/**
 * The AbstractHive class ...
 */
class AbstractHive {

public:
    AbstractHive();
    AbstractHive(const HiveConfig& hc);
    virtual ~AbstractHive() {}

private:
    float m_fPosX;
    float m_fPosY;
};

#endif /* _ABSTRACTHIVE_H */
