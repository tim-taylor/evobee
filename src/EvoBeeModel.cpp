/**
 * @file
 *
 * Implementation of the EvoBeeModel class
 */

#include <iostream>
#include "EvoBeeModel.h"

using namespace std;

EvoBeeModel::EvoBeeModel() :
    m_iStep(0)
{
}

/**
@todo
*/
void EvoBeeModel::step() {
    m_iStep++;
    cout << "Model step " << m_iStep << endl;
}