/**
 * @file
 *
 * Implementation of the HoneyBee class
 */

#include <iostream>
#include <exception>
#include "FloweringPlant.h"
#include "Environment.h"
#include "ModelParams.h"
#include "Flower.h"
#include "HoneyBee.h"

std::string HoneyBee::m_sTypeNameStr{"HNB"};


HoneyBee::HoneyBee(const PollinatorConfig& pc, AbstractHive* pHive) :
    Hymenoptera(pc, pHive)
{
}

HoneyBee::HoneyBee(const HoneyBee& other) :
    Hymenoptera(other)
{
}

HoneyBee::HoneyBee(HoneyBee&& other) noexcept :
    Hymenoptera(std::move(other))
{
}

void HoneyBee::reset()
{
    Hymenoptera::reset();
}

std::string HoneyBee::getStateString() const
{
    return Hymenoptera::getStateString();
}


const std::string& HoneyBee::getTypeName() const
{
    return m_sTypeNameStr;
}
