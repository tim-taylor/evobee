/**
 * @file
 *
 * Implementation of the HoneyBee class
 */

#include <iostream>
#include "HoneyBee.h"

HoneyBee::HoneyBee(AbstractHive* pHive) :
    Hymenoptera(pHive)
{
}

void HoneyBee::step()
{
    ///@todo...
    std::cout << "Stepping a bee!" << std::endl;
}