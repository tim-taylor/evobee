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

    /**
     *
     */
    std::string getStateString() const override final;

    /**
     *
     */
    const std::string& getTypeName() const override final;

private:
    /**
     * String description of the HoneyBee type, for use in log files
     */
    static std::string m_sTypeNameStr;    
};

#endif /* _HONEYBEE_H */
