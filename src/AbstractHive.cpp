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
    m_Position(hc.position),
    m_bStartFromHive(hc.startFromHive),
    m_InitForageAreaTopLeft(hc.areaTopLeft),
    m_InitForageAreaBottomRight(hc.areaBottomRight),
    m_bMigrationAllowed(hc.migrationAllowed),
    m_fMigrationProb(hc.migrationProb)
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
    return m_pEnv->getRandomPositionF(m_InitForageAreaTopLeft, m_InitForageAreaBottomRight);
}