/**
 * @file
 *
 * Implementation of the Pollinator class
 */


#include "Pollinator.h"

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