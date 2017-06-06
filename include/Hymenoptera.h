/**
 * @file
 *
 * Declaration of the Hymenoptera class
 */

#ifndef _HYMENOPTERA_H
#define _HYMENOPTERA_H

#include <string>
#include "Pollinator.h"


/**
 * The Hymenoptera class ...
 */
class Hymenoptera : public Pollinator {

public:
    Hymenoptera(AbstractHive* pHive);
    //~Hymenoptera() {}

    void step() override = 0;

    std::string getStateString() const override;

    const std::string& getTypeName() const override;

private:
    /*
    innate colour pref
    learned colour pref
    */

    /**
     * String description of the Hymenoptera type, for use in log files
     */    
    static std::string m_sTypeNameStr;    
};

#endif /* _HYMENOPTERA_H */
