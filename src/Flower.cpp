/**
 * @file
 *
 * Implementation of the Flower class
 */

#include "Flower.h"


Flower::Flower(const PlantTypeConfig& ptc, fPos pos, MarkerPoint mp) :
    m_Position(pos),
    m_Reflectance(mp),
    m_bPollinated(false),
    m_iAntherPollen(ptc.antherInitPollen),
    m_iAntherPollenLossPerVisit(ptc.antherPollenLossPerVisit),
    m_iStigmaPollen(0),
    m_iStigmaMaxPollenCapacity(ptc.stigmaMaxPollenCapacity),
    //m_iNectarRewardPerVisit(ptc.nectarReward),
    m_fTemp(ptc.initTemp)
{}

