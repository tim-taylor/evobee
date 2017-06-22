/**
 * @file
 *
 * Implementation of the Flower class
 */

#include <iostream>
#include <algorithm>
#include "FloweringPlant.h"
#include "Flower.h"

unsigned int Flower::m_sNextFreeId = 1;


Flower::Flower(FloweringPlant* pPlant, const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp) :
    m_id(m_sNextFreeId++),
    m_SpeciesId(pPlant->getSpeciesId()),
    m_Position(pos),
    m_Reflectance(mp),
    m_bPollinated(false),
    m_iAntherPollen(ptc.antherInitPollen),
    m_fTemperature(ptc.initTemp),
    m_pPlant(pPlant),
    m_iAntherPollenTransferPerVisit(ptc.antherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity),
    m_bPollenClogging(ptc.pollenClogging)
    //m_iNectarRewardPerVisit(ptc.nectarReward)
{}


const std::string& Flower::getSpecies() const
{
    return m_pPlant->getSpecies();
}


int Flower::transferAntherPollenToPollinator(PollenVector& pollinatorStore)
{
    // calculate how many grains to transfer
    int num = std::min(m_iAntherPollenTransferPerVisit, m_iAntherPollen);

    // remove that number from the anther...
    m_iAntherPollen -= num;

    // ... and add it to the pollinator's store
    // (NB this is where we actually create Pollen objects)
    // Note that we do not check the pollinatorStore's maximum capacity here,
    // this is the responsibility of the pollinator after it has called this
    // method.
    for (int i = 0; i < num; ++i)
    {
        pollinatorStore.emplace_back(this, m_SpeciesId);
    }

    // return the number of grains transferred
    return num;
}


// Attempt to transfer the requested number of Pollen grains from the specified
// pollinator's store to the flower's stigma. If any of the deposited pollen
// is from the same species, the flower will be pollinated.
//
int Flower::transferPollenFromPollinator(PollenVector& pollinatorStore, int suggestedNum)
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
    if (m_bPollenClogging)
    {
        // No restriction on pollen species, so we can grab any pollen from
        // the pollinator. Note that the pollinator's store should already be shuffled, 
        // so we can just pick the required number of grains from the end of the
        // store, and this will be a random selection
        actualNum = attemptedNum;
    }
    else    
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
                if (nextItr->pSource->getSpeciesId() == m_SpeciesId)
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

    // Whether m_bPollenClogging is true or false, we have now figured out how many grains
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
            Pollen* pP = nullptr; // test code to allow output of pollen info

            if (m_bPollenClogging)
            {
                // pollen could be of any species, so we need to see whether any of
                // the new pollen is actually from the same species as this flower
                auto it = std::find_if( m_StigmaPollen.end()-actualNum,
                                        m_StigmaPollen.end(),
                                        [this](Pollen& p){return (p.speciesId == this->m_SpeciesId);} );
                m_bPollinated = (it != m_StigmaPollen.end());

                // test code to allow output of pollen info
                if (m_bPollinated) pP = &(*it);
            }
            else
            {
                // in the case of no pollen clogging, only pollen from this species is allowed
                // on the stigma, so we know for sure that the flower is now pollinated
                m_bPollinated = true;

                // test code to allow output of pollen info
                pP = &(*(m_StigmaPollen.end()-actualNum));
            }

            // test code
            if (m_bPollinated)
            {
                std::cout << "Flower id " << m_id << " of species " << m_SpeciesId <<
                    " pollinated by pollen of species " << pP->speciesId <<
                    " from plant id " << pP->pSource->getId() << std::endl;
            }
            else std::cout << "(no pollination)" << std::endl;
        }        
    }

    // Finally, compare the number of grains actually deposited on the stigma with the amount
    // of the requested transfer. If the actual amount is less, then the pollinator just 
    // loses the difference anyway
    if (actualNum < suggestedNum)
    {
        // first ensure we don't try to lose more than is actually in the pollinator's store
        int numToLose = std::min({ suggestedNum-actualNum, (int)pollinatorStore.size() });
        pollinatorStore.erase(pollinatorStore.end()-numToLose, pollinatorStore.end());
    }

    return actualNum;
}