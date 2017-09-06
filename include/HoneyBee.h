/**
 * @file
 *
 * Declaration of the HoneyBee class
 */

#ifndef _HONEYBEE_H
#define _HONEYBEE_H

#include "Hymenoptera.h"

class Flower;


/**
 * The HoneyBee class ...
 */
class HoneyBee : public Hymenoptera {

public:
    HoneyBee(const PollinatorConfig& pc, AbstractHive* pHive);
    ~HoneyBee() {}

    HoneyBee(const HoneyBee& other);
    HoneyBee(HoneyBee&& other) noexcept;

    /**
     * Peform a complete reset of this pollinator to its initial state,
     * resetting position and all other state variables as if this
     * was a brand new pollinator
     */
    void reset() override final;

    /**
     *
     */
    //void step() override final;

    /**
     *
     */
    std::string getStateString() const override final;

    /**
     *
     */
    const std::string& getTypeName() const override final;


protected:
    /**
     * Determine whether the pollinator should harvest the specified flower
     */
     //bool isVisitCandidate(Flower* pFlower) const;

    /**
     *
     */
    //void visitFlower(Flower* pFlower);


private:
    /**
     * String description of the HoneyBee type, for use in log files
     */
    static std::string m_sTypeNameStr;

};

#endif /* _HONEYBEE_H */
