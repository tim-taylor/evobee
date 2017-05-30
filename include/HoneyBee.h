/**
 * @file
 *
 * Declaration of the HoneyBee class
 */

#ifndef _HONEYBEE_H
#define _HONEYBEE_H

#include "Hymenoptera.h"

/**
 * The HoneyBee class ...
 */
class HoneyBee : public Hymenoptera {

public:
    HoneyBee(AbstractHive* pHive);
    ~HoneyBee() {}

    /**
     *
     */
    void step() override final;
};

#endif /* _HONEYBEE_H */
