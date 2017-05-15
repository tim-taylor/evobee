/**
 * @file
 *
 * Implementation of the EvoBeeModel class
 */

#include <iostream>
#include "Environment.h"
#include "EvoBeeModel.h"

using std::cout;
using std::endl;

EvoBeeModel::EvoBeeModel() : 
    m_iStep(0),
    m_Env()
{
    ///@todo initialise Environment correctly
}

/**
@todo
*/
void EvoBeeModel::step()
{
    m_iStep++;
    cout << "Model step " << m_iStep << endl;
}
