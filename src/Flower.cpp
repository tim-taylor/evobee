/**
 * @file
 *
 * Implementation of the Flower class
 */

#include <iostream>
#include <algorithm>
#include <exception>
#include <cassert>
#include "FloweringPlant.h"
#include "Flower.h"

unsigned int Flower::m_sNextFreeId = 1;


// create a new flower based upon initial config info
Flower::Flower( FloweringPlant* pPlant, 
                const PlantTypeConfig& ptc,
                const fPos& pos,
                MarkerPoint mp ) :
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


// create a new flower based upon a flower from the parent plant
Flower::Flower( FloweringPlant* pPlant, 
                const Flower& parentFlower,
                const fPos& pos,
                const ReflectanceInfo& reflectance ) :
    m_id(m_sNextFreeId++),
    m_SpeciesId(pPlant->getSpeciesId()),
    m_Position(pos),
    m_Reflectance(reflectance),
    m_bPollinated(false),
    m_iAntherPollen(pPlant->m_pPlantTypeConfig->antherInitPollen),
    m_fTemperature(pPlant->m_pPlantTypeConfig->initTemp),
    m_pPlant(pPlant),
    m_iAntherPollenTransferPerVisit(pPlant->m_pPlantTypeConfig->antherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(pPlant->m_pPlantTypeConfig->stigmaMaxPollenCapacity),
    m_bPollenClogging(pPlant->m_pPlantTypeConfig->pollenClogging)
    ////m_iNectarRewardPerVisit(ptc.nectarReward)
{}


///////////////////////////////////////
// copy constructor
Flower::Flower(const Flower& other) :
    m_id(m_sNextFreeId++),          // for copy constructor we assign a new id
    m_SpeciesId(other.m_SpeciesId),
    m_Position(other.m_Position),
    m_Reflectance(other.m_Reflectance),
    m_bPollinated(other.m_bPollinated),
    m_iAntherPollen(other.m_iAntherPollen),
    m_StigmaPollen(other.m_StigmaPollen),
    m_fTemperature(other.m_fTemperature),
    m_pPlant(other.m_pPlant),
    m_iAntherPollenTransferPerVisit(other.m_iAntherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(other.m_iStigmaMaxPollenCapacity),
    m_bPollenClogging(other.m_bPollenClogging)
{
    // NB we should not be in a situation where we are making a copy of a Flower
    // with non-empty m_StigmaPollen. The only time when the Flower copy constructor
    // might get called is when we're initially creating it and putting it in the
    // owning FloweringPlant's m_Flowers vector.
    if (!m_StigmaPollen.empty())
    {
        throw std::runtime_error("Attempt to copy an old Flower! Something is badly wrong...");
    }
}

// move constructor
Flower::Flower(Flower&& other) noexcept :
    m_id(other.m_id),                // for move constructor we keep the same id
    m_SpeciesId(other.m_SpeciesId),
    m_Position(other.m_Position),
    m_Reflectance(other.m_Reflectance),
    m_bPollinated(other.m_bPollinated),
    m_iAntherPollen(other.m_iAntherPollen),
    m_StigmaPollen(other.m_StigmaPollen),
    m_fTemperature(other.m_fTemperature),
    m_pPlant(other.m_pPlant),
    m_iAntherPollenTransferPerVisit(other.m_iAntherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(other.m_iStigmaMaxPollenCapacity),
    m_bPollenClogging(other.m_bPollenClogging)
{
    other.m_id = 0;
}

Flower::~Flower() noexcept
{
}

// copy assignment operator
Flower& Flower::operator= (const Flower& other)
{
    // We shouldn't be in a position where we are copying Flower objects using the assignment
    // operator. So we throw an exception here, for debugging purposes
    throw std::runtime_error("Attempt to copy an old Flower by assigment operator - not expecting this!");
    
    m_id = m_sNextFreeId++; // for copy assignment we assign a new id   
    copyCommon(other);
    
    // NB we should not be in a situation where we are making a copy of a Flower
    // with non-empty m_StigmaPollen. The only time when the Flower copy assignment
    // might get called is when we're initially creating it and putting it in the
    // owning FloweringPlant's m_Flowers vector.
    if (!m_StigmaPollen.empty())
    {
        throw std::runtime_error("Attempt to copy an old Flower! Something is badly wrong...");
    }

    return *this;
}

// move assignment operator
Flower& Flower::operator= (Flower&& other) noexcept
{
    // We shouldn't be in a position where we are copying Flower objects using the assignment
    // operator. So we assert false here, for debugging purposes
    assert(false);

    m_id = other.m_id;      // for move assignment we keep the same id
    other.m_id = 0;
    copyCommon(other);
    return *this;
}

void Flower::copyCommon(const Flower& other) noexcept
{  
    m_SpeciesId = other.m_SpeciesId;
    m_Position = other.m_Position;
    m_Reflectance = other.m_Reflectance;
    m_bPollinated = other.m_bPollinated;
    m_iAntherPollen = other.m_iAntherPollen;
    m_StigmaPollen = other.m_StigmaPollen; 
    m_fTemperature = other.m_fTemperature;
    m_pPlant = other.m_pPlant;

    // We can't copy the following at present, because they are consts!
    // If we wanted to do this, we'd have to find another way...
    
    //m_iAntherPollenTransferPerVisit = other.m_iAntherPollenTransferPerVisit;
    //m_iStigmaMaxPollenCapacity = other.m_iStigmaMaxPollenCapacity;
    //m_bPollenClogging = other.m_bPollenClogging;
}
///////////////////////////////////////


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

            if (m_bPollinated)
            {
                m_pPlant->setPollinated();

                // test code
                std::cout << "Flower id " << m_id << " of species " << m_SpeciesId <<
                    " pollinated by pollen of species " << pP->speciesId <<
                    " from plant id " << pP->pSource->getId() << std::endl;
            }
            else 
            {
                // test code
                std::cout << "(no pollination)" << std::endl;
            }
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