/**
 * @file
 *
 * Implementation of the ModelParams class
 */

#include "ModelParams.h"

// Instantiate all static members
bool   ModelParams::m_bVisualisation = true;
int    ModelParams::m_iVisUpdatePeriod = 1;
int    ModelParams::m_iEnvSizeX = 50;
int    ModelParams::m_iEnvSizeY = 50;
float  ModelParams::m_fEnvDefaultAmbientTemp = 20.0;
MarkerPoint ModelParams::m_EnvBackgroundReflectanceMP = 400;
float  ModelParams::m_fMaxScreenFracH = 0.8;
float  ModelParams::m_fMaxScreenFracW = 0.8;
bool   ModelParams::m_bInitialised = false;
int    ModelParams::m_iTerminationNumSteps = 100;
std::string ModelParams::m_strRngSeed {""};
std::string ModelParams::m_strLogDir {"output"};
std::string ModelParams::m_strLogRunName {"run"};

std::vector<HiveConfig> ModelParams::m_Hives;
std::vector<PlantTypeDistributionConfig> ModelParams::m_PlantDists;
std::vector<PlantTypeConfig> ModelParams::m_PlantTypes;

nlohmann::json ModelParams::m_Json;


void ModelParams::setEnvSize(int x, int y)
{
    if (x > 0) m_iEnvSizeX = x;
    if (y > 0) m_iEnvSizeY = y;
}

void ModelParams::setEnvSizeX(int x)
{
    if (x > 0) m_iEnvSizeX = x;
}

void ModelParams::setEnvSizeY(int y)
{
    if (y > 0) m_iEnvSizeY = y;
}

void ModelParams::setVisualisation(bool vis)
{
    m_bVisualisation = vis;
}

/**
*/
void ModelParams::setMaxScreenFrac(float f)
{
    ModelParams::setMaxScreenFracW(f);
    ModelParams::setMaxScreenFracH(f);
}

void ModelParams::setMaxScreenFracW(float fw)
{
    if (fw < 0.1)
    {
        m_fMaxScreenFracW = 0.1;
        /// @todo... maybe output warning msg here and in next case?
    }
    else if (fw > 1.0)
    {
        m_fMaxScreenFracW = 1.0;
    }
    else 
    {
        m_fMaxScreenFracW = fw;
    }
}

void ModelParams::setMaxScreenFracH(float fh)
{
    if (fh < 0.1)
    {
        m_fMaxScreenFracH = 0.1;
        /// @todo... maybe output warning msg here and in next case?
    }
    else if (fh > 1.0)
    {
        m_fMaxScreenFracH = 1.0;
    }
    else 
    {
        m_fMaxScreenFracH = fh;
    }    
}

void ModelParams::setEnvDefaultAmbientTemp(float temp)
{
    m_fEnvDefaultAmbientTemp = temp;
}

void ModelParams::setEnvBackgroundReflectanceMP(MarkerPoint mp)
{
    m_EnvBackgroundReflectanceMP = mp;
}

void ModelParams::setVisUpdatePeriod(int p)
{
    if (p > 0)
        m_iVisUpdatePeriod = p;
}

void ModelParams::setTerminationNumSteps(int steps)
{
    m_iTerminationNumSteps = steps;
}

void ModelParams::setRngSeedStr(const std::string& seed)
{
    m_strRngSeed = seed;
}

void ModelParams::setLogDir(const std::string& dir)
{
    m_strLogDir = dir;
}

void ModelParams::setLogRunName(const std::string& name)
{
    m_strLogRunName = name;
}

void ModelParams::setInitialised()
{
    m_bInitialised = true;
}

void ModelParams::addHiveConfig(HiveConfig& hc)
{
    m_Hives.push_back(hc);
}

void ModelParams::addPlantTypeDistributionConfig(PlantTypeDistributionConfig& pc)
{
    m_PlantDists.push_back(pc);
}

void ModelParams::addPlantTypeConfig(PlantTypeConfig& pt)
{
    m_PlantTypes.push_back(pt);
}

const PlantTypeConfig* ModelParams::getPlantTypeConfig(std::string species)
{
    for (auto& pt : m_PlantTypes)
    {
        if (pt.species == species) return &pt;
    }
    return nullptr;
}