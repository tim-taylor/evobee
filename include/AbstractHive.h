/**
 * @file
 *
 * Declaration of the Hive class
 */

#ifndef _ABSTRACTHIVE_H
#define _ABSTRACTHIVE_H

#include <memory>
#include "HiveConfig.h"


/**
 * The AbstractHive class ...
 */
class AbstractHive {

public:
    //AbstractHive();
    AbstractHive(const HiveConfig& hc);
    virtual ~AbstractHive() {}

    /*
     * A static factory method to produce a Hive of the subtype specified
     * by hc.type, and return a shared pointer to the created object
     *
     * 
     */
    static std::shared_ptr<AbstractHive> makeHive(const HiveConfig& hc);

private:
    float m_fPosX;
    float m_fPosY;
};

#endif /* _ABSTRACTHIVE_H */
