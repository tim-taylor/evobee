/**
 * @file
 *
 * Declaration of the EvoBeeModel class
 */

#ifndef _EVOBEEMODEL_H
#define _EVOBEEMODEL_H

#include <random>
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

    /// Get current step number
    unsigned int getStepNumber() const {return m_iStep;}

    /// Return a reference to the model's Environment object
    Environment& getEnv() {return m_Env;}

    /// Seed the model's RNG from the seed specified in ModelParams
    static void seedRng();

    /// EvoBeeModel owns an RNG engine for use by all components in the model
    static std::mt19937 m_sRngEngine;

private:
    unsigned int    m_iStep;    ///< Current step number
    Environment     m_Env;      ///< The model owns the one and only 

    static bool m_sbRngInitialised;
};

#endif /* _EVOBEEMODEL_H */
