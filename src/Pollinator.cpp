/**
 * @file
 *
 * Implementation of the Pollinator class
 */

#include <cmath>
#include <random>
#include <cassert>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include "tools.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "PollinatorConfig.h"
#include "Pollinator.h"

unsigned int Pollinator::m_sNextFreeId = 1;
std::string Pollinator::m_sTypeNameStr{"POL"};


Pollinator::Pollinator(const PollinatorConfig& pc, AbstractHive* pHive) :
    m_id(m_sNextFreeId++),
    m_pHive(pHive),
    m_pEnv(nullptr),
    m_pModel(nullptr),
    m_State(PollinatorState::UNINITIATED),
    m_iNumFlowersVisitedInBout(0),
    m_iBoutLength(pc.boutLength),
    m_iPollenDepositPerFlowerVisit(pc.pollenDepositPerFlowerVisit),
    m_iPollenLossInAir(pc.pollenLossInAir),
    m_iMaxPollenCapacity(pc.maxPollenCapacity),
    m_iPollenCarryoverNumVisits(pc.pollenCarryoverNumVisits)
{
    // Each pollinator has a pointer to the environment and to the model for
    // convenience, to save having to ask the Hive for these each time we need them
    m_pEnv = m_pHive->getEnvironment();
    m_pModel = m_pEnv->getModel();

    // Set this pollinators innate preference for a flower colour marker point
    if (pc.innateMPPrefMin == pc.innateMPPrefMax)
    {
        m_InnateMPPref = pc.innateMPPrefMin;
    }
    else
    {
        // pick a marker point at uniform random between the min and max values supplied
        std::uniform_int_distribution<MarkerPoint> dist(pc.innateMPPrefMin, pc.innateMPPrefMax);
        m_InnateMPPref = dist(EvoBeeModel::m_sRngEngine);
    }

    // Set the starting position and heading of the pollinator
    resetToStartPosition();

    if (ModelParams::logPollinatorsSummary())
    {
        // initialise PerformanceInfoMap by adding an entry for each species
        const std::map<unsigned int, std::string>& speciesInfoMap = FloweringPlant::getSpeciesMap();
        for (auto& speciesInfo : speciesInfoMap)
        {
            m_PerformanceInfoMap.emplace(speciesInfo.first, PollinatorPerformanceInfo());
        }
    }
}


// copy constructor
Pollinator::Pollinator(const Pollinator& other) :
    m_id(m_sNextFreeId++),          // for copy constructor we assign a new id
    m_Position(other.m_Position),
    m_fHeading(other.m_fHeading),
    m_pHive(other.m_pHive),
    m_pEnv(other.m_pEnv),
    m_pModel(other.m_pModel),
    m_State(other.m_State),
    m_iNumFlowersVisitedInBout(other.m_iNumFlowersVisitedInBout),
    m_PollenStore(other.m_PollenStore),
    m_MovementAreaTopLeft(other.m_MovementAreaTopLeft),
    m_MovementAreaBottomRight(other.m_MovementAreaBottomRight),
    m_InnateMPPref(other.m_InnateMPPref),
    m_iBoutLength(other.m_iBoutLength),
    m_iPollenDepositPerFlowerVisit(other.m_iPollenDepositPerFlowerVisit),
    m_iPollenLossInAir(other.m_iPollenLossInAir),
    m_iMaxPollenCapacity(other.m_iMaxPollenCapacity),
    m_iPollenCarryoverNumVisits(other.m_iPollenCarryoverNumVisits),
    m_PerformanceInfoMap(other.m_PerformanceInfoMap)
{
    // NB we should not be in a situation where we are making a copy of a Pollinator
    // with non-empty m_PollenStore. The only time when the Pollinator copy constructor
    // might get called is when we're initially creating it and putting it in the
    // owning Hive's m_Pollinators vector.
    if (!m_PollenStore.empty())
    {
        throw std::runtime_error("Attempt to copy an old Pollinator! Something is badly wrong...");
    }
}


// move constructor
Pollinator::Pollinator(Pollinator&& other) noexcept :
    m_id(other.m_id),                // for move constructor we keep the same id
    m_Position(other.m_Position),
    m_fHeading(other.m_fHeading),
    m_pHive(other.m_pHive),
    m_pEnv(other.m_pEnv),
    m_pModel(other.m_pModel),
    m_State(other.m_State),
    m_iNumFlowersVisitedInBout(other.m_iNumFlowersVisitedInBout),
    m_PollenStore(std::move(other.m_PollenStore)),
    m_MovementAreaTopLeft(other.m_MovementAreaTopLeft),
    m_MovementAreaBottomRight(other.m_MovementAreaBottomRight),
    m_InnateMPPref(other.m_InnateMPPref),
    m_iBoutLength(other.m_iBoutLength),
    m_iPollenDepositPerFlowerVisit(other.m_iPollenDepositPerFlowerVisit),
    m_iPollenLossInAir(other.m_iPollenLossInAir),
    m_iMaxPollenCapacity(other.m_iMaxPollenCapacity),
    m_iPollenCarryoverNumVisits(other.m_iPollenCarryoverNumVisits),
    m_PerformanceInfoMap(other.m_PerformanceInfoMap)
{
    other.m_id = 0;
}


// destructor
Pollinator::~Pollinator() noexcept
{
}


// copy assignment operator
Pollinator& Pollinator::operator= (const Pollinator& other)
{
    // We shouldn't be in a position where we are copying Pollinator objects using the assignment
    // operator. So we throw an exception here, for debugging purposes
    throw std::runtime_error("Attempt to copy an old Pollinator by assigment operator - not expecting this!");

    return *this;
}


// move assignment operator
Pollinator& Pollinator::operator= (Pollinator&& other) noexcept
{
    // We shouldn't be in a position where we are copying Flower objects using the assignment
    // operator. So we assert false here, for debugging purposes
    assert(false);

    return *this;
}


void Pollinator::reset()
{
    m_State = PollinatorState::UNINITIATED;
    m_iNumFlowersVisitedInBout = 0;
    m_PollenStore.clear();
    for (auto& perfInfo : m_PerformanceInfoMap)
    {
        perfInfo.second.reset();
    }
    resetToStartPosition();
    ///@todo Should m_InnateMPPref be reset in Pollinator::reset? If so, we'll need to
    // store the originally specified min and max values passed into the constructor
    // from PollinatorConfig. It's possible that we might want either the value itself,
    // or the min and max values of the range, to evolve over time as well.
}


void Pollinator::resetToStartPosition()
{
    m_fHeading = EvoBeeModel::m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);

    if (m_pHive->startFromHive())
    {
        m_Position = m_pHive->getPosition();
    }
    else
    {
        m_Position = m_pHive->getRandomPollinatorStartPosition();
    }

    resetMovementArea();
}


void Pollinator::resetMovementArea()
{
    assert(m_pEnv != nullptr);

    const Patch& patch = m_pEnv->getPatch(m_Position);
    m_MovementAreaTopLeft = patch.getReproRestrictionAreaTopLeft();
    m_MovementAreaBottomRight = patch.getReproRestrictionAreaBottomRight();
}


/*
 * Query whether this pollinator is currently within its allowed area,
 * taking into account whether migration is allowed to outside of the
 * Hive's initial area.
 * NB This method also considers the migration restrictions defined for the
 * Hive. If the pollinator has moved out of its previously allowed movement
 * area and the migration is deemed to be allowed, then the pollinator's
 * allowed movement area is updated according to the reproRestructionArea of
 * the new patch. Hence, this is NOT a const method.
 */
bool Pollinator::inAllowedArea()
{
    bool ok = ((m_Position.x >= (float)m_MovementAreaTopLeft.x) &&
               (m_Position.x <  (float)(m_MovementAreaBottomRight.x+1)) &&
               (m_Position.y >= (float)m_MovementAreaTopLeft.y) &&
               (m_Position.y <  (float)(m_MovementAreaBottomRight.y+1)));

    if ((!ok) && m_pHive->migrationAllowed())
    {
        if ((!m_pHive->migrationRestricted()) ||
            (EvoBeeModel::m_sUniformProbDistrib(EvoBeeModel::m_sRngEngine) < m_pHive->migrationProb()))
        {
            if (inEnvironment())
            {
                ok = true;
                resetMovementArea();
            }
        }
    }

    return ok;
}


// A helper method for the move... methods for use when a pollinator has
// wandered off the environment. Reflect the movement off the edge of
// the environment and reposition the pollinator back within the allowed
// area
void Pollinator::repositionInEnv(fPos delta)
{
    repositionInArea(delta, 0.0, 0.0, m_pEnv->getSizeXf(), m_pEnv->getSizeYf());
    assert(inEnvironment());
}


// A helper method for the moveXXX methods for use when a pollinator has
// wandered out of its allowed area. Reflect the movement off the edge of
// the environment and reposition the pollinator back within the allowed
// area
//
// @param delta is the change in position that the pollinator was trying to
// make from its current position, which would take it outside of its
// allowed area
void Pollinator::repositionInAllowedArea(fPos delta)
{
    /*
    const iPos& TL = m_pHive->getInitForageAreaTopLeft();
    const iPos& BR = m_pHive->getInitForageAreaBottomRight();
    repositionInArea(delta, (float)TL.x, (float)TL.y, (float)(BR.x+1), (float)(BR.y+1));
    */

    repositionInArea(delta,
        (float)(m_MovementAreaTopLeft.x),
        (float)(m_MovementAreaTopLeft.y),
        (float)(m_MovementAreaBottomRight.x+1),
        (float)(m_MovementAreaBottomRight.y+1));

    assert(inEnvironment());
}


// A helper method for the move... methods for use when a pollinator has
// wandered out of the specified area. Reflect the movement off the edge of
// the environment and reposition the pollinator back within the allowed
// area
//
// @param delta is the change in position that the pollinator was trying to
// make from its current position, which would take it outside of its
// allowed area
void Pollinator::repositionInArea(fPos delta, float minx, float miny, float maxx, float maxy)
{
    fPos oldPos = m_Position - delta;

    // to place the pollinator back in its allowed area, we'll need to reverse either
    // the x or y component of its proposed move (or maybe both)
    if (m_Position.x < minx || m_Position.x >= maxx)
    {
        delta.x = -delta.x;
    }

    if (m_Position.y < miny || m_Position.y >= maxy)
    {
        delta.y = -delta.y;
    }

    // now perform the movement with the adjusted delta
    m_Position = oldPos + delta;

    // as a belt and braces measure, do one final check that the
    // new position is within the allowed area
    if (m_Position.x < minx)
    {
        m_Position.x = minx;
    }
    else if (m_Position.x >= maxx)
    {
        m_Position.x = maxx - (2.0 * EvoBee::FLOAT_COMPARISON_EPSILON);
    }
    if (m_Position.y < miny)
    {
        m_Position.y = miny;
    }
    else if (m_Position.y >= maxy)
    {
        m_Position.y = maxy - (2.0 * EvoBee::FLOAT_COMPARISON_EPSILON);
    }
}


// Move by a given distance in a random direction.
//
// @todo Maybe implement a "stay/hover" option in future?
//
void Pollinator::moveRandom(/*bool allowOffEnv,*/ float stepLength)
{
    m_fHeading = EvoBeeModel::m_sDirectionDistrib(EvoBeeModel::m_sRngEngine);

    fPos delta{stepLength*std::cos(m_fHeading), stepLength*std::sin(m_fHeading)};
    m_Position += delta;

    if (!inAllowedArea())
    {
        repositionInAllowedArea(delta);
    }
}


bool Pollinator::moveBiassed(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveBiassed
    fPos delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;
}


bool Pollinator::moveLevy(bool allowOffEnv, float stepLength)
{
    ///@todo implement moveLevy
    fPos delta;

    bool inEnv = inEnvironment();

    if (!inEnv && !allowOffEnv)
    {
        repositionInEnv(delta);
    }

    return inEnv;
}


// for each Pollen grain in the store, update its landing count
void Pollinator::updatePollenLandingCount()
{
    std::for_each( m_PollenStore.begin(),
                   m_PollenStore.end(),
                   [](Pollen& p){p.numLandings++;} );
}


// Remove any pollen from the store that has exceeded m_iPollenCarryoverNumVisits
void Pollinator::removeOldCarryoverPollen()
{
    m_PollenStore.erase(
        std::remove_if( m_PollenStore.begin(),
                        m_PollenStore.end(),
                        [this](Pollen& p) {return (p.numLandings > m_iPollenCarryoverNumVisits);} ),
        m_PollenStore.end()
    );
}


// Transfer some of our pollen to the flower (potentially pollinating it)
// Also, if we are in a logging mode that cares about it, update the
// pollinator's performance info about number of landings and pollinations
// of the specified flower species.
int Pollinator::depositPollenOnStigma(Flower* pFlower)
{
    int numGrainsDeposited = 0;
    PollinatorPerformanceInfo* pPerfInfo = nullptr;
    bool pollinatedBefore;

    if (ModelParams::logPollinatorsSummary())
    {
        auto& perfInfo = m_PerformanceInfoMap.at(pFlower->getSpeciesId());
        //auto& perfInfo = m_PerformanceInfoMap[pFlower->getSpeciesId())];
        pPerfInfo = &perfInfo;
        pPerfInfo->numLandings++;
        pollinatedBefore = pFlower->pollinated();
    }

    numGrainsDeposited = pFlower->transferPollenFromPollinator( m_PollenStore,
                                                                m_iPollenDepositPerFlowerVisit );

    if (ModelParams::logPollinatorsSummary())
    {
        if ((!pollinatedBefore) && (pFlower->pollinated()))
        {
            pPerfInfo->numPollinations++;
        }
    }

    return numGrainsDeposited;
}


// Collect pollen from flower if available, and add to our store
// If any pollen is added, the contents of the store is shuffled so that
// subsequent operations that act of randomly chosen grains from the store
// can simply and efficiently pick off grains from the end of the store.
// If the store's maxium capacity is exceeded, some random grains are
// removed to bring it down to the maximum allowed size.
//
void Pollinator::collectPollenFromAnther(Flower* pFlower)
{
    // transfer anther pollen from flower to our store
    int num = pFlower->transferAntherPollenToPollinator(m_PollenStore);

    // if some new grains have been added, shuffle the whole store now
    // so that subsequent removals of pollen happen in a random order
    if (num > 0)
    {
        std::shuffle(m_PollenStore.begin(), m_PollenStore.end(), EvoBeeModel::m_sRngEngine);
    }

    // finally, if the number of grains in our store now exceeds the
    // maximum capacity, delete some. As we've just shuffled the store,
    // we can just remove the required number from the end of the store
    int xs = m_PollenStore.size() - m_iMaxPollenCapacity;
    if (xs > 0)
    {
        m_PollenStore.erase(m_PollenStore.end()-xs, m_PollenStore.end());
    }
}


// Lose the specified amount of pollen to the air
int Pollinator::losePollenToAir(int num)
{
    num = std::min(num, (int)m_PollenStore.size());
    m_PollenStore.erase(m_PollenStore.end()-num, m_PollenStore.end());
    return num;
}


std::string Pollinator::getStateString() const
{
    std::stringstream ssState;
    ssState << std::fixed << std::setprecision(3) << getTypeName() << ","
        << m_id << "," << m_Position.x << "," << m_Position.y << "," << m_fHeading;
    return ssState.str();
}


const std::string& Pollinator::getTypeName() const
{
    return m_sTypeNameStr;
}


int Pollinator::getNumPollenGrainsInStore(unsigned int speciesId) const
{
    return std::count_if( m_PollenStore.begin(),
                          m_PollenStore.end(),
                          [speciesId](const Pollen& p) {return (p.speciesId == speciesId);} );
}
