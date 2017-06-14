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


void HoneyBee::step()
{
    switch (m_State)
    {
        case (PollinatorState::UNINITIATED):
        {
            m_State = PollinatorState::FORAGING;
            // and now fall through to next case
        }
        case (PollinatorState::FORAGING):
        {
            // first move
            ///@todo... need to parameterise type of movement and details
            moveRandom();

            // now look for flowers nearby
            ///@todo - for now we are looking for nearest plant, not nearest flower
            /// (so assuming plants just have one flower)
            bool flowerVisited = false;

            FloweringPlant *pPlant = getEnvironment()->findClosestFloweringPlant(m_Position);
            if (pPlant != nullptr)
            {
                Flower* pFlower = pPlant->getFlower(0);
                if (isVisitCandidate(pFlower))
                {
                    visitFlower(pFlower);
                    flowerVisited = true;
                }
            }

            if (!flowerVisited)
            {
                losePollenToAir(m_iPollenLossInAir);
            }

            break;
        }
        case (PollinatorState::BOUT_COMPLETE):
        {
            // (do nothing)
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown pollinator state encountered");
        }

    }
}


void HoneyBee::visitFlower(Flower* pFlower)
{
    // for each Pollen grain in the store, update its landing count
    updatePollenLandingCount();

    // remove pollen from store that is now past the carryover limit
    removeOldCarryoverPollen();

    // first transfer some of bee's pollen to the flower (potentially pollinating it)
    depositPollenOnStigma(pFlower);

    // now pick up more pollen from the flower
    collectPollenFromAnther(pFlower);

    // update count of number of flowers visited, and end bout if done
    if (++m_iNumFlowersVisitedInBout >= m_iBoutLength)
    {
        m_State = PollinatorState::BOUT_COMPLETE;
    }
}


bool HoneyBee::isVisitCandidate(Flower* pFlower) const
{
    bool bOfInterest = true;
    ///@todo - implement isHarvestCandidate based upon innate and learned preferences for flower colour

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

    return bOfInterest;
}


std::string HoneyBee::getStateString() const
{
    return Hymenoptera::getStateString();
}


const std::string& HoneyBee::getTypeName() const
{
    return m_sTypeNameStr;
}
