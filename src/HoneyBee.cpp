/**
 * @file
 *
 * Implementation of the HoneyBee class
 */

#include <iostream>
#include <exception>
#include "FloweringPlant.h"
#include "Environment.h"
#include "ModelParams.h"
#include "Flower.h"
#include "HoneyBee.h"

std::string HoneyBee::m_sTypeNameStr{"HNB"};


HoneyBee::HoneyBee(const PollinatorConfig& pc, AbstractHive* pHive) :
    Hymenoptera(pc, pHive)
{
}

HoneyBee::HoneyBee(const HoneyBee& other) :
    Hymenoptera(other)
{
}

HoneyBee::HoneyBee(HoneyBee&& other) noexcept :
    Hymenoptera(std::move(other))
{
}

void HoneyBee::reset()
{
    Hymenoptera::reset();
}


/*
bool HoneyBee::isVisitCandidate(Flower* pFlower) const
{
    / **
    // from Zoe's code...

    //Ensusre that is it more likely to harvest this flower if it is the right colour/is favoured, and vice versa.

    Flower flower = field.getFlowerAtLocation(row,col);			//The flower in question
    Flower.FlowerType type = flower.getType();				//Its type
    double sensitivity = flowerTypeSensitivity.get(type);			//Sensitivity to this flowers colour
    double currentLearnedPreference = flowerTypeLearnedPreference.get(type);//And this bee's currrent preference for it

    if(Math.random() < (sensitivity + currentLearnedPreference)) //(Math.random() < field.getFlowerAtLocation(row,col).getHymSensitivity())
    {
        harvest = true;
        //TRIAL updateLearnedPreferences(type);	//If it lands here increase it preference for this flower
        //System.out.printf("YES!"+flowerTypeLearnedPreference);

    }
    else
    {
        //System.out.printf("NOPE!"); 	//Don't touch the preferences if not harvesting
    }
    ** /

    return bOfInterest;
}
*/

std::string HoneyBee::getStateString() const
{
    return Hymenoptera::getStateString();
}


const std::string& HoneyBee::getTypeName() const
{
    return m_sTypeNameStr;
}
