/**
 * @file
 *
 * Implementation of the Flower class
 */

#include <iostream>
#include "Flower.h"


Flower::Flower(const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp) :
    m_Position(pos),
    m_Reflectance(mp),
    m_bPollinated(false),
    m_iAntherPollen(ptc.antherInitPollen),
    m_fTemp(ptc.initTemp),
    m_iAntherPollenLossPerVisit(ptc.antherPollenLossPerVisit),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity)
    //m_iNectarRewardPerVisit(ptc.nectarReward)
{}


int Flower::transferAntherPollenToPollinator(PollenVector& pollinatorStore)
{
    // calculate how many grains to transfer
    int num = std::min(m_iAntherPollenLossPerVisit, m_iAntherPollen);

    // remove that number from the anther
    m_iAntherPollen -= num;

    // and add it to the pollinator's store
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


// 
int Flower::transferPollenFromPollinator(PollenVector& pollinatorStore, int num)
{
    //int num = 0;

    ///@todo

    // check whether the flower has been pollinated
    ///@todo
    m_bPollinated = true;

    std::cout << "Pollinated!" << std::endl;

    return num;
}