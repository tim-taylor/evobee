/**
 * @file
 *
 * Implementation of the Environment class
 */

#include <cassert>
#include <vector>
#include <iostream>
#include <memory>
#include "ModelParams.h"
#include "Patch.h"
#include "Colour.h"
#include "AbstractHive.h"
#include "Hive.h"
#include "HoneyBee.h"
#include "Environment.h"

using std::cout;
using std::endl;

Environment::Environment() 
{
    assert(ModelParams::initialised());

    // Initialise Patches
    m_iNumPatches = ModelParams::getNumPatches();
    m_Patches.reserve(m_iNumPatches);
    for (int i = 0; i < m_iNumPatches; ++i) 
    {
        m_Patches.push_back(
            Patch(this, i, 
                ModelParams::getEnvDefaultBackgroundColour(),
                ModelParams::getEnvDefaultAmbientTemp()
            )
        );
    }

    // Initialise Hives
    const std::vector<HiveConfig> & hconfigs = ModelParams::getHiveConfigs();
    for (const HiveConfig& hconfig : hconfigs)
    {
        m_Hives.push_back( AbstractHive::makeHive(hconfig) );
    }

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