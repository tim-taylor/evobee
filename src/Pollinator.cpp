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
std::uniform_real_distribution<float> Pollinator::m_sDirectionDistrib(0.0, TWOPI);


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
    m_fHeading = m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);

    if (m_pHive->startFromHive())
    {
        m_Position = m_pHive->getPosition();
    }
    else
    {
        m_Position = m_pHive->getRandomPollinatorStartPosition();
    }
}


// A helper method for the move... methods for use when a pollinator has
// wandered off the environment. Reflect the movement off the edge of
// the environment and reposition the pollinator back within the allowed
// area
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


// Move by a given distance in a random direction. 
//
// @todo Maybe implement a "stay/hover" option in future?
//
bool Pollinator::moveRandom(bool allowOffEnv, float stepLength)
{
    m_fHeading = m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);

    fPos delta{stepLength*std::cos(m_fHeading), stepLength*std::sin(m_fHeading)};
    m_Position += delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;
}


bool Pollinator::moveBiassed(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveBiassed
    fPos delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;    
}

bool Pollinator::moveLevy(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveLevy
    fPos delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;    
}

std::string Pollinator::getStateString() const
{
    std::stringstream ssState;
    ssState << std::fixed << std::setprecision(3) << getTypeName() << ","
        << m_id << "," << m_Position.x << "," << m_Position.y << "," << m_fHeading;
    return ssState.str();
}

const std::string& Pollinator::getTypeName() const
{
    return m_sTypeNameStr;
} 