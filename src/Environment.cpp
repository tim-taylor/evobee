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
#include "Colour.h"
#include "Environment.h"

using std::cout;
using std::endl;

Environment::Environment() 
{
    assert(ModelParams::initialised());

    // Initialise Patches
    Colour defaultColour(10,140,10); /// @todo will want to set this from config file

    m_iNumPatches = ModelParams::getNumPatches();
    m_Patches.reserve(m_iNumPatches);
    for (int i = 0; i < m_iNumPatches; ++i) 
    {
        m_Patches.push_back(Patch(this, i, defaultColour, ModelParams::getEnvDefaultAmbientTemp()));
    }

    // Initialise Hives
    ///@todo

    // Initialise Plants
    ///@todo

    cout << "Initialised Environment with " << m_Patches.size() << " patches" << endl;    
}

void Environment::getPatchCoordsFromIdx(int idx, int* retX, int* retY)
{
    assert(idx >= 0);
    assert(idx < m_iNumPatches);
    *retX = idx % ModelParams::getEnvSizeX();
    *retY = idx / ModelParams::getEnvSizeX();
}