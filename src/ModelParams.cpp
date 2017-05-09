/**
 * @file
 *
 * Implementation of the ModelParams class
 */

#include "ModelParams.h"

// Instantiate all static members
bool  ModelParams::m_bVisualisation = true;
int   ModelParams::m_iVisUpdatePeriod = 1;
int   ModelParams::m_iEnvSizeX = 50;
int   ModelParams::m_iEnvSizeY = 50;
float ModelParams::m_fMaxScreenFracH = 0.8;
float ModelParams::m_fMaxScreenFracW = 0.8;
bool  ModelParams::m_bInitialised = false;


void ModelParams::setEnvSize(int x, int y)
{
    if (x > 0) m_iEnvSizeX = x;
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

void ModelParams::setVisUpdatePeriod(int p)
{
    if (p > 0)
        m_iVisUpdatePeriod = p;
}

void ModelParams::setInitialised()
{
    m_bInitialised = true;
}