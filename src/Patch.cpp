/**
 * @file
 *
 * Implementation of the Patch class
 */

#include <iostream>
#include "Environment.h"
#include "Patch.h"

using std::cout;
using std::endl;

Patch::Patch(Environment* pEnv, int posIdx, MarkerPoint mp, float temp) :
    m_pEnv(pEnv),
    m_fTemp(temp),
    m_posIdx(posIdx),
    m_BackgroundReflectance(mp)
{
    m_Position = m_pEnv->getPatchCoordsFromIdx(posIdx);
}

void Patch::addPlant(const PlantTypeConfig& ptc, fPos pos)
{
    ///@todo (and will need more params!)
    cout << "Adding plant of species " << ptc.species << " to Patch [" << m_Position << "] at coordinates (" << pos << ")" << endl;

    m_FloweringPlants.push_back(FloweringPlant(ptc, pos));
}