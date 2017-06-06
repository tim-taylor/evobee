/**
 * @file
 *
 * Implementation of the Pollinator class
 */

#include <cmath>
#include <random>
#include <cassert>
#include <string>
#include <sstream>
#include <iomanip>
#include "EvoBeeModel.h"
#include "Pollinator.h"

constexpr double PI = std::atan(1)*4.0;
constexpr double TWOPI = 2.0*PI;

unsigned int Pollinator::m_sNextFreeId = 0;
std::string Pollinator::m_sTypeNameStr{"POL"};


Pollinator::Pollinator(AbstractHive* pHive) :
    m_id(m_sNextFreeId++),
    m_pHive(pHive)
{
    resetToStartPosition();

    // Each pollinator has a pointer to the environment for convenience, to 
    // save having to ask the Hive for the pointer each time we need it
    m_pEnv = m_pHive->getEnvironment(); 
}


void Pollinator::resetToStartPosition()
{
    if (m_pHive->startFromHive())
    {
        m_Position = m_pHive->getPosition();
    }
    else
    {
        m_Position = m_pHive->getRandomPollinatorStartPosition();
    }
}


bool Pollinator::moveRandom(bool allowOffEnv, float stepLength)
{
    std::uniform_real_distribution<float> dist(0.0, TWOPI);
    float ang = dist(EvoBeeModel::m_sRngEngine);

    fPos delta { stepLength * std::cos(ang), stepLength * std::sin(ang) };
    m_Position += delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;
}


void Pollinator::repositionInEnv(fPos delta)
{
    fPos oldPos = m_Position - delta;

    if (m_Position.x < 0.0 || m_Position.x >= m_pEnv->getSizeXf())
    {
        delta.x = -delta.x;
    }

    if (m_Position.y < 0.0 || m_Position.y >= m_pEnv->getSizeYf())
    {
        delta.y = -delta.y;
    }

    m_Position = oldPos + delta;

    assert(inEnvironment());
}


bool Pollinator::moveBiassed(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveBiassed
}

bool Pollinator::moveLevy(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveLevy
}

std::string Pollinator::getStateString() const
{
    std::stringstream ssState;
    ssState << std::fixed << std::setprecision(3) << getTypeName() << ","
        << m_id << "," << m_Position.x << "," << m_Position.y;
    return ssState.str();
}

const std::string& Pollinator::getTypeName() const
{
    return m_sTypeNameStr;
} 