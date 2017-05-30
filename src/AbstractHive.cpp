/**
 * @file
 *
 * Implementation of the AbstractHive class
 */

#include <memory>
#include <stdexcept>
#include "Environment.h"
#include "HoneyBee.h"
#include "Hive.h"
#include "AbstractHive.h"


AbstractHive::AbstractHive(Environment* pEnv, const HiveConfig &hc) :
    m_pEnv(pEnv),
    m_Position(hc.x, hc.y),
    m_bStartFromHive(hc.startFromHive)
{
}

std::shared_ptr<AbstractHive> AbstractHive::makeHive(Environment* pEnv, const HiveConfig& hc)
{
    if (hc.type == "HoneyBee")
    {
        return std::make_shared<Hive<HoneyBee>>(pEnv, hc);
    }
    /*
    else if (hc.type == "Bumblebee")
    {
        return std::make_shared<Hive<BumbleBee>>(hc);
    }
    */ 
    else
    {
        throw std::runtime_error("Unknown Hive type " + hc.type + " in config file");
    }
}

fPos AbstractHive::getRandomPollinatorStartPosition() const
{
    return m_pEnv->getRandomPositionF();
}