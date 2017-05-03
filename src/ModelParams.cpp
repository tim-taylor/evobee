/**
 * @file ModelParams.cpp
 *
 * A desciption of the ModelParams.cpp file...
 */

#include "ModelParams.h"


bool ModelParams::m_bVisualisation = true;  ///< Use visualiation for this run?
int  ModelParams::m_iEnvSizeX = 50;         ///< Environment size (num patches) in x direction
int  ModelParams::m_iEnvSizeY = 50;         ///< Environment size (num patches) in y direction
float ModelParams::m_fMaxScreenFracH = 0.8; ///< Max fraction of screen size for window height
float ModelParams::m_fMaxScreenFracW = 0.8; ///< Max fraction of screen size for window width


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
