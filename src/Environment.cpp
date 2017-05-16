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
#include "HoneyBeeHive.h"
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
    const std::vector<HiveConfig> & hcs = ModelParams::getHiveConfigs();
    for (const HiveConfig& hc : hcs)
    {
        // Okay, we need a Hive factory here to make a hive of the right type

        // temp test code...
        auto hp = std::make_shared<HoneyBeeHive<HoneyBee>>(hc);

        m_Hives.push_back( hp );
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