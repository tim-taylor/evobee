/**
 * @file
 *
 * Implementation of the Flower class
 */

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
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
    m_iAvailableNectar(ptc.initNectar),
    m_fTemperature(ptc.initTemp),
    m_pPlant(pPlant),
    m_iAntherPollenTransferPerVisit(ptc.antherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity),
    m_bPollenCloggingAll(ptc.pollenCloggingAll),
    m_bPollenCloggingPartial(ptc.pollenCloggingPartial),
    m_CloggingSpeciesVec(pPlant->getCloggingSpeciesVec())
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
    m_iAvailableNectar(pPlant->m_pPlantTypeConfig->initNectar),
    m_fTemperature(pPlant->m_pPlantTypeConfig->initTemp),
    m_pPlant(pPlant),
    m_iAntherPollenTransferPerVisit(pPlant->m_pPlantTypeConfig->antherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(pPlant->m_pPlantTypeConfig->stigmaMaxPollenCapacity),
    m_bPollenCloggingAll(pPlant->m_pPlantTypeConfig->pollenCloggingAll),
    m_bPollenCloggingPartial(pPlant->m_pPlantTypeConfig->pollenCloggingPartial),
    m_CloggingSpeciesVec(parentFlower.m_CloggingSpeciesVec)
{}


// copy constructor
Flower::Flower(const Flower& other) :
    m_id(m_sNextFreeId++),          // for copy constructor we assign a new id
    m_SpeciesId(other.m_SpeciesId),
    m_Position(other.m_Position),
    m_Reflectance(other.m_Reflectance),
    m_bPollinated(other.m_bPollinated),
    m_iAntherPollen(other.m_iAntherPollen),
    m_StigmaPollen(other.m_StigmaPollen),
    m_iAvailableNectar(other.m_iAvailableNectar),
    m_fTemperature(other.m_fTemperature),
    m_pPlant(other.m_pPlant),
    m_iAntherPollenTransferPerVisit(other.m_iAntherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(other.m_iStigmaMaxPollenCapacity),
    m_bPollenCloggingAll(other.m_bPollenCloggingAll),
    m_bPollenCloggingPartial(other.m_bPollenCloggingPartial),
    m_CloggingSpeciesVec(other.m_CloggingSpeciesVec)
{
    // NB we should not be in a situation where we are making a copy of a Flower
    // with non-empty m_StigmaPollen. The only time when the Flower copy constructor
    // might get called is when we're initially creating it and putting it in the
    // owning FloweringPlant's m_Flowers vector.
    if (!m_StigmaPollen.empty())
    {
        throw std::runtime_error("Attempt to copy an old Flower! Something is badly wrong... (copy ctr)");
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
    m_StigmaPollen(std::move(other.m_StigmaPollen)),
    m_iAvailableNectar(other.m_iAvailableNectar),
    m_fTemperature(other.m_fTemperature),
    m_pPlant(other.m_pPlant),
    m_iAntherPollenTransferPerVisit(other.m_iAntherPollenTransferPerVisit),
    m_iStigmaMaxPollenCapacity(other.m_iStigmaMaxPollenCapacity),
    m_bPollenCloggingAll(other.m_bPollenCloggingAll),
    m_bPollenCloggingPartial(other.m_bPollenCloggingPartial),
    m_CloggingSpeciesVec(other.m_CloggingSpeciesVec)
{
    other.m_id = 0;
}


//destructor
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
        throw std::runtime_error("Attempt to copy an old Flower! Something is badly wrong... (copy asgnmt op)");
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


// helper method used by copy/move assignment operators
void Flower::copyCommon(const Flower& other) noexcept
{
    // N.B. we don't expect this method to get called in the current code... the methods that call
    // it now throw an exception or assertion failure before calling this!
    assert(false);
    //throw std::runtime_error("Attempt to call a flower's copyCommon method - not expecting this!");

    /*
    m_SpeciesId = other.m_SpeciesId;
    m_Position = other.m_Position;
    m_Reflectance = other.m_Reflectance;
    m_bPollinated = other.m_bPollinated;
    m_iAntherPollen = other.m_iAntherPollen;
    m_StigmaPollen = other.m_StigmaPollen;
    m_iAvailableNectar = other.m_iAvailableNectar;
    m_fTemperature = other.m_fTemperature;
    m_pPlant = other.m_pPlant;
    m_iAntherPollenTransferPerVisit = other.m_iAntherPollenTransferPerVisit;
    m_iStigmaMaxPollenCapacity = other.m_iStigmaMaxPollenCapacity;
    m_bPollenCloggingAll = other.m_bPollenCloggingAll;
    m_bPollenCloggingPartial = other.m_bPollenCloggingPartial;
    //m_CloggingSpeciesVec = other.m_CloggingSpeciesVec; // can't copy this as it is constant!
    */
}


std::string Flower::getStateString() const
{
    std::stringstream ssState;
    ssState << std::fixed << std::setprecision(3)
        << m_id << "," << m_SpeciesId << "," << m_Position.x << "," << m_Position.y << ","
        << getMarkerPoint() << "," << (m_bPollinated ? "P":"N") << ","
        << m_iAntherPollen << "," << m_StigmaPollen.size() << "," << m_iAvailableNectar;
    return ssState.str();
}


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
// is from the same species, the flower will be pollinated. If less pollen gets
// transferred to the flower than the requested number, the difference between
// the requested and actual number of pollen grains transferred is removed
// from the pollinator's store and treated as lost.
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
    if (FloweringPlant::cloggingAll())
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
                if (FloweringPlant::pollenTransferToStigmaAllowed(nextItr->pSource, this))
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
    // are to be moved, and the grains we want to move are now at the end of the pollinatorStore.
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
            if (FloweringPlant::cloggingNone())
            {
                // in the case of no pollen clogging, only pollen from this species is allowed
                // on the stigma, so we know for sure that the flower is now pollinated
                m_bPollinated = true;
            }
            else
            {
                // pollen could be of any species, so we need to see whether any of
                // the new pollen is actually from the same species as this flower
                auto it = std::find_if( m_StigmaPollen.end()-actualNum,
                                        m_StigmaPollen.end(),
                                        [this](Pollen& p){return (p.speciesId == this->m_SpeciesId);} );
                m_bPollinated = (it != m_StigmaPollen.end());
            }

            if (m_bPollinated)
            {
                m_pPlant->setPollinated();
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


// Respond to a pollinator's request for nectar
int Flower::collectNectar(int amountRequested)
{
    int amountGiven = std::min(amountRequested, m_iAvailableNectar);
    m_iAvailableNectar -= amountGiven;
    return amountGiven;
}