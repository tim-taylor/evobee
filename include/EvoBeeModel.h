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
    //~EvoBeeModel() {}

    /**
     * Iterate the simulation by one step
     */
    void step();

    /**
    * Initialise a new generation.
    * We need to construct a new generation of plants based upon those successfully
    * pollinated in the previous generation, taking into acconut any refuges and/or
    * restrictions to seed flow.
    */
    void initialiseNewGeneration();

    /**
     * Get current generation number
     */
    unsigned int getGenNumber() const {return m_iGen;}

    /**
     * Get current simulation step number (within the current generation)
     */
    unsigned int getStepNumber() const {return m_iStep;}

    /**
     * Return a reference to the model's Environment object
     */
    Environment& getEnv() {return m_Env;}

    /**
     * Seed the model's RNG from the seed specified in ModelParams
     */
    static void seedRng();

    /**
     * EvoBeeModel owns an RNG engine for use by all components in the model
     */
    static std::mt19937 m_sRngEngine;

    static std::uniform_real_distribution<float> m_sDirectionDistrib;   ///< Uniform distrib 0.0--TWOPI

    static std::uniform_real_distribution<float> m_sUniformProbDistrib; ///< Uniform distrib 0.0--1.0

    static std::cauchy_distribution<float> m_sCauchyProbDistrib;        ///< Standard Cauchy distrib, shift 0.0, scale 1.0

private:
    unsigned int    m_iGen;     ///< Current generation number
    unsigned int    m_iStep;    ///< Current step number within current generation
    Environment     m_Env;      ///< The model owns the one and only

    static bool m_sbRngInitialised;
};

#endif /* _EVOBEEMODEL_H */
