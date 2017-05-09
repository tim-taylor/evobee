/**
 * @file
 *
 * Implementation of the Patch class
 */

#include <iostream>
#include "Environment.h"
#include "Patch.h"

using namespace std;

Patch::Patch(Environment* pEnv, int posIdx) :
    m_pEnv(pEnv),
    m_posIdx(posIdx)
{
    m_pEnv->getPatchCoordsFromIdx(posIdx, &m_posX, &m_posY);
    cout << "Initialised patch at (" << m_posX << "," << m_posY << ")" << endl;
}
