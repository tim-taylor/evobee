/**
 * @file
 *
 * Implementation of the EvoBeeModel class
 */

#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include "Environment.h"
#include "Pollinator.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"

// Define some commonly used constant expressions
constexpr double PI = 4.0*std::atan(1);
constexpr double TWOPI = 2.0*PI;


// Define EvoBeeModel's static members
// -- create our static random number generator engine
std::mt19937 EvoBeeModel::m_sRngEngine;
bool EvoBeeModel::m_sbRngInitialised = false;
// -- and define some commonly used distributions
std::uniform_real_distribution<float> EvoBeeModel::m_sDirectionDistrib(0.0, TWOPI);
std::uniform_real_distribution<float> EvoBeeModel::m_sUniformProbDistrib(0.0, 1.0);


EvoBeeModel::EvoBeeModel() :
    m_iGen(0),
    m_iStep(0),
    m_Env(this)
{
    assert(ModelParams::initialised());
    assert(m_sbRngInitialised);
}

/**
 * Static method to seed the model's RNG generator from the seed in ModelParams
 * ModelParams should be initialised before this method is called
 * This method should be called before an EvoBeeModel instance is instantiated
 */
void EvoBeeModel::seedRng()
{
    assert(ModelParams::initialised());
    assert(!m_sbRngInitialised);

    const std::string& seedStr = ModelParams::getRngSeedStr();

    if (seedStr.empty()) {
        // if no seed string has been supplied, we generate a seed here
        // We keep it consistent with the format of user-supplied seeds by creating
        // a random string of alphanumeric characters (of length 20).
        std::srand( std::random_device()() );
        std::string alphanum {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
        std::string newSeedStr(20,0);
        std::generate(newSeedStr.begin(),
                      newSeedStr.end(),
                      [alphanum]() { return alphanum[rand() % alphanum.length()]; });

        //std::cout << "Using generated RNG seed " << newSeedStr << std::endl;
        
        // having generated a seed string, now use it to seed the RNG!
        std::seed_seq seed1(newSeedStr.begin(), newSeedStr.end());   
        m_sRngEngine.seed(seed1);
        
        // and store the generated seed string back in ModelParams
        ModelParams::setRngSeedStr(newSeedStr, true);
    }
    else
    {
        std::seed_seq seed2(seedStr.begin(), seedStr.end());
        m_sRngEngine.seed(seed2);
    }

    m_sbRngInitialised = true;
}


/**
 * Run one step of the simulation for the current generation.
 * This entails running one step of all current pollinators in the environment.
 */
void EvoBeeModel::step()
{
    std::cout << "Model gen " << m_iGen << ", step " << m_iStep << std::endl;

    // first allow all pollinators to update
    auto pollinators = m_Env.getAllPollinators();
    std::shuffle(pollinators.begin(), pollinators.end(), m_sRngEngine);
    for (Pollinator* pol : pollinators)
    {
        pol->step();
    }

    ++m_iStep;    
}


/**
 * Initialise a new generation. 
 * We need to construct a new generation of plants based upon those successfully
 * pollinated in the previous generation, taking into acconut any refuges and/or
 * restrictions to seed flow.
 */
void EvoBeeModel::initialiseNewGeneration()
{
    ++m_iGen;
    m_Env.initialiseNewGeneration();
    m_iStep = 0;
}
