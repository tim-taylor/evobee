/**
 * @file
 *
 * Implementation of the HoneyBee class
 */

#include <iostream>
#include "FloweringPlant.h"
#include "Environment.h"
#include "HoneyBee.h"

HoneyBee::HoneyBee(AbstractHive* pHive) :
    Hymenoptera(pHive)
{
}

void HoneyBee::step()
{
    ///@todo... need to parameterise type of movement and details

    // first move
    moveRandom();

    // now look for flowers nearby
    FloweringPlant *pPlant = getEnvironment()->findClosestFloweringPlant(m_Position);
    if (pPlant != nullptr)
    {
        // near a plant
    }
}