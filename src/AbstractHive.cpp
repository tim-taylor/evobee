/**
 * @file
 *
 * Implementation of the AbstractHive class
 */

#include <memory>
#include <stdexcept>
#include "HoneyBee.h"
#include "Hive.h"
#include "AbstractHive.h"

/*
AbstractHive::AbstractHive() {
}
*/

AbstractHive::AbstractHive(const HiveConfig &hc) :
    m_fPosX(hc.x),
    m_fPosY(hc.y),
    m_bStartFromHive(true)
{
}

std::shared_ptr<AbstractHive> AbstractHive::makeHive(const HiveConfig& hc)
{
    /*
    if (hc.type == "Bumblebee")
    {
        return std::make_shared<Hive<BumbleBee>>(hc);
    }
    else*/ if (hc.type == "HoneyBee")
    {
        return std::make_shared<Hive<HoneyBee>>(hc);
    }
    else
    {
        throw std::runtime_error("Unknown Hive type " + hc.type + " in config file");
    }
}