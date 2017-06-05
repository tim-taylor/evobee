/**
 * @file
 *
 * Implementation of the Pollinator class
 */

#include <cmath>
#include <random>
#include "EvoBeeModel.h"
#include "Pollinator.h"

constexpr double PI = std::atan(1) * 4;

Pollinator::Pollinator(AbstractHive* pHive) :
    m_pHive(pHive)
{
    resetToStartPosition();
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

void Pollinator::moveRandom()
{
    float stepLength = 1.0;
    std::uniform_real_distribution<float> dist(0.0, 2.0 * PI);
    float ang = dist(EvoBeeModel::m_sRngEngine);
    fPos delta { stepLength * std::cos(ang), stepLength * std::sin(ang) };
    m_Position += delta;
    ///@todo may want to chack bounds now, or maybe sometimes we don't?
}

void Pollinator::moveBiassed()
{
    
}

void Pollinator::moveLevy()
{
    
}