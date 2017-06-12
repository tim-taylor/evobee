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
#include "HoneyBee.h"

std::string HoneyBee::m_sTypeNameStr{"HNB"};


HoneyBee::HoneyBee(const PollinatorConfig& pc, AbstractHive* pHive) :
    Hymenoptera(pc, pHive)
{
}

void HoneyBee::step()
{
    ///@todo... need to parameterise type of movement and details

    // first move
    moveRandom();

    // now look for flowers nearby
    ///@todo - for now we are looking for nearest plant, not nearest flower
    /// (so assuming plants just have one flower)
    FloweringPlant *pPlant = getEnvironment()->findClosestFloweringPlant(m_Position);
    if (pPlant != nullptr)
    {
        Flower* pFlower = pPlant->getFlower(0);
        if (isHarvestCandidate(pFlower))
        {
            ///@todo
            //std::cout << "num num..." << std::endl;
        }
    }
}

bool HoneyBee::isHarvestCandidate(Flower* pFlower) const
{
    bool bHarvestable = true;
    ///@todo

    /*
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
    */

    return bHarvestable;
}


std::string HoneyBee::getStateString() const
{
    return Hymenoptera::getStateString();
}


const std::string& HoneyBee::getTypeName() const
{
    return m_sTypeNameStr;
}
