/**
 * @file
 *
 * Declaration of the AbstractHive class
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
     */
    static std::shared_ptr<AbstractHive> makeHive(const HiveConfig& hc);

private:
    float m_fPosX;          ///< Hive position in environment on x axis
    float m_fPosY;          ///< Hive position in environment on y axis
    bool  m_bStartFromHive; ///< Do Pollinators start from hive, or at a random position?
};

#endif /* _ABSTRACTHIVE_H */
