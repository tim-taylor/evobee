/**
 * @file
 *
 * Implementation of the Environment class
 */

#include <cassert>
#include <vector>
#include <iostream>
#include "ModelParams.h"
#include "Patch.h"
#include "Environment.h"

using namespace std;

Environment::Environment() {
    assert(ModelParams::initialised());

    m_iNumPatches = ModelParams::getNumPatches();
    m_Patches.reserve(m_iNumPatches);
    for (int i = 0; i < m_iNumPatches; ++i) 
    {
        m_Patches.push_back(Patch(this, i));
    }
    cout << "Initialised Environment with " << m_Patches.size() << " patches" << endl;
}

void Environment::getPatchCoordsFromIdx(int idx, int* retX, int* retY) {
    assert(idx >= 0);
    assert(idx < m_iNumPatches);
    *retX = idx % ModelParams::getEnvSizeX();
    *retY = idx / ModelParams::getEnvSizeX();
}