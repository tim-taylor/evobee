/**
 * @file
 *
 * Declaration of the EvoBeeModel class
 */

#ifndef _EVOBEEMODEL_H
#define _EVOBEEMODEL_H

#include "Environment.h"

/**
 * The EvoBeeModel class ...
 */
class EvoBeeModel {

public:
    EvoBeeModel();
    ~EvoBeeModel() {}

    /// Iterate the simulation by one step
    void step();

    /// Return a reference to the model's Environment object
    Environment& getEnv() {return m_Env;}

private:
    int m_iStep;
    Environment m_Env; ///< The model owns the one and only 
};

#endif /* _EVOBEEMODEL_H */
