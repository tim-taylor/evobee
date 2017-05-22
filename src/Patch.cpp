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

Patch::Patch(Environment* pEnv, int posIdx, const Colour& col, float temp) :
    m_pEnv(pEnv),
    m_Colour(col),
    m_fTemp(temp),
    m_posIdx(posIdx)
{
    m_pEnv->getPatchCoordsFromIdx(posIdx, &m_posX, &m_posY);
    //cout << "Initialised patch at (" << m_posX << "," << m_posY << ")" << endl;
}

void Patch::addPlant(float x, float y)
{
    ///@todo (and will need more params!)
    cout << "Adding plant to Patch [" << m_posX << "," << m_posY << "] at coordinates (" << x << "," << y << ")" << endl;
}