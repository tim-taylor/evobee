/**
 * @file
 *
 * Implementation of the Hymenoptera class
 */

#include <string>
#include "Hymenoptera.h"

std::string Hymenoptera::m_sTypeNameStr{"HYM"};


Hymenoptera::Hymenoptera(AbstractHive* pHive) :
    Pollinator(pHive)
{
}


std::string Hymenoptera::getStateString() const
{
    return Pollinator::getStateString();
}


const std::string& Hymenoptera::getTypeName() const
{
    return m_sTypeNameStr;
} 