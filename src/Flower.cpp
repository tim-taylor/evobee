/**
 * @file
 *
 * Implementation of the Flower class
 */

#include <iostream>
#include <algorithm>
#include "FloweringPlant.h"
#include "Flower.h"


Flower::Flower(FloweringPlant* pPlant, const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp) :
    m_Position(pos),
    m_Reflectance(mp),
    m_bPollinated(false),
    m_iAntherPollen(ptc.antherInitPollen),
    m_fTemp(ptc.initTemp),
    m_pPlant(pPlant),
    m_iAntherPollenLossPerVisit(ptc.antherPollenLossPerVisit),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity)
    //m_iNectarRewardPerVisit(ptc.nectarReward)
{}


const std::string& Flower::getSpecies() const
{
    return m_pPlant->getSpecies();
}


int Flower::transferAntherPollenToPollinator(PollenVector& pollinatorStore)
{
    // calculate how many grains to transfer
    int num = std::min(m_iAntherPollenLossPerVisit, m_iAntherPollen);

    // remove that number from the anther...
    m_iAntherPollen -= num;

    // ... and add it to the pollinator's store
    // (NB this is where we actually create Pollen objects)
    // Note that we do not check the pollinatorStore's maximum capacity here,
    // this is the responsibility of the pollinator after it has called this
    // method.
    for (int i = 0; i < num; ++i)
    {
        pollinatorStore.push_back( Pollen(this) );
    }

    // return the number of grains transferred
    return num;
}


// Attempt to transfer the requested number of Pollen grains from the specified
// pollinator's store to the flower's stigma. If any of the deposited pollen
// is from the same species, the flower will be pollinated.
//
int Flower::transferPollenFromPollinator(PollenVector& pollinatorStore,
                                         int suggestedNum,
                                         bool sameSpeciesOnly)
{
    // first figure out how many grains we should attempt to transfer,
    // taking into account the suggested number from the pollinator, the
    // remaining capacity of the stigma, and the amount of pollen actually
    // being carried by the pollinator
    int attemptedNum = std::min({suggestedNum, 
                                 m_iStigmaMaxPollenCapacity - (int)m_StigmaPollen.size(),
                                 (int)pollinatorStore.size()});
    
    int actualNum = 0;
    
    // now calculate how many grains we can actually move from the pollinatorStore,
    // taking account of the pollen species if necessary, and ensure that all grains
    // to be moved are placed at the end of the pollinatorStore ready for being
    // moved in bulk
    if (sameSpeciesOnly)
    {
        // We can only transfer pollen of the same species as this flower, so we need
        // to look through the pollinator's store and select which grains we will take.
        // For efficiency, we do this by first searching through the pollinator's store
        // and moving any matching pollen to the end of the store (we exit this process
        // when we've found enough pollen, or when we've reached the end of the store).
        if (!pollinatorStore.empty())
        {
            auto nextItr = pollinatorStore.begin();
            auto swapItr = pollinatorStore.end();

            while ((nextItr != swapItr) && (actualNum < attemptedNum))
            {
                if (nextItr->pSource->getSpecies() == getSpecies())
                {
                    --swapItr;
                    if (nextItr != swapItr)
                    {
                        std::iter_swap(nextItr, swapItr);
                    }
                    ++actualNum;
                }
                else
                {
                    ++nextItr;
                }
            }
        }
    }
    else
    {
        // No restriction on pollen species, so we can grab any pollen from
        // the pollinator. Note that the pollinator's store should already be shuffled, 
        // so we can just pick the required number of grains from the end of the
        // store, and this will be a random selection
        actualNum = attemptedNum;
    }

    // Whether sameSpeciesOnly is true or false, we have now figured out how many grains
    // are to moved, and the grains we want to move are now at the end of the pollinatorStore.
    // Now we actually move those grains to the stigma store in one operation, and delete
    // them from the pollinatorStore.
    if (actualNum > 0)
    {
        std::move(pollinatorStore.end()-actualNum,
                  pollinatorStore.end(),
                  std::back_inserter(m_StigmaPollen));
        
        pollinatorStore.erase(pollinatorStore.end()-actualNum, pollinatorStore.end());

        // if not already pollinated, check whether that has now changed!
        if (!m_bPollinated)
        {
            if (sameSpeciesOnly)
            {
                m_bPollinated = true;
            }
            else
            {
                auto it = std::find_if( m_StigmaPollen.end()-actualNum,
                                        m_StigmaPollen.end(),
                                        [this](Pollen& p){return (p.pSource->getSpecies() == getSpecies());} );
                m_bPollinated = (it != m_StigmaPollen.end());
            }

            // temp debgugging message...
            if (m_bPollinated)
            {
                std::cout << "Pollinated!" << std::endl;
            }        
        }        
    }

    return actualNum;
}