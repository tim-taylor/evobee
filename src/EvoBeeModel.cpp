/**
 * @file
 *
 * Implementation of the EvoBeeModel class
 */

#include <iostream>
#include <random>
#include <cassert>
#include "Environment.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"

using std::cout;
using std::endl;

// Create our static random number generator engine
std::mt19937 EvoBeeModel::m_sRngEngine;


EvoBeeModel::EvoBeeModel() : 
    m_iStep(0),
    m_Env()
{
    assert(ModelParams::initialised());
    m_sRngEngine.seed(ModelParams::getRngSeed());
}


/**
 * @todo
 */
void EvoBeeModel::step()
{
    m_iStep++;
    cout << "Model step " << m_iStep << endl;
}
