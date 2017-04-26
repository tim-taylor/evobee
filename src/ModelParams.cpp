/**
 * @file ModelParams.cpp
 *
 * A desciption of the ModelParams.cpp file...
 */

#include "ModelParams.h"


bool ModelParams::m_bVisualisation = true;  ///< Use visualiation for this run?
int  ModelParams::m_iEnvSizeX = 50;         ///< Environment size (num patches) in x direction
int  ModelParams::m_iEnvSizeY = 50;         ///< Environment size (num patches) in y direction


void ModelParams::setEnvSize(int x, int y)
{
    if (x > 0) m_iEnvSizeX = x;
    if (y > 0) m_iEnvSizeY = y;
}

void ModelParams::setVisualisation(bool vis)
{
    m_bVisualisation = vis;
}
