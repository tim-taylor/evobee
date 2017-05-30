/**
 * @file
 *
 * Declaration of the Hymenoptera class
 */

#ifndef _HYMENOPTERA_H
#define _HYMENOPTERA_H

#include "Pollinator.h"

/**
 * The Hymenoptera class ...
 */
class Hymenoptera : public Pollinator {

public:
    Hymenoptera(AbstractHive* pHive);
    virtual ~Hymenoptera() {}

private:
    /*
    innate colour pref
    learned colour pref
    */
};

#endif /* _HYMENOPTERA_H */
