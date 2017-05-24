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
#include "ModelParams.h"
#include "EvoBeeModel.h"

using std::cout;
using std::endl;

// Create our static random number generator engine
std::mt19937 EvoBeeModel::m_sRngEngine;
bool EvoBeeModel::m_sbRngInitialised = false;


EvoBeeModel::EvoBeeModel() : 
    m_iStep(0),
    m_Env()
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

    static std::string strPrefix {"seqseed-"};
    static std::string alphanum {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};

    const std::string& seedStr = ModelParams::getRngSeedStr();
    if (seedStr.empty()) {
        // if no seed string has been supplied, we generate a seed here
        std::random_device rdev;
        uint32_t random_seed = rdev();
        std::srand(random_seed);

        std::string newSeedStr(20,0);
        std::generate_n(newSeedStr.begin(), 
                        newSeedStr.length(),
                        []() { return alphanum[ rand() % alphanum.length() ]; } );
        std::seed_seq seed1(newSeedStr.begin(), newSeedStr.end());

        std::cout << "Using generated RNG seed " << newSeedStr << std::endl;
        
        m_sRngEngine.seed(seed1);
        
        // and store the generated seed back in ModelParams
        ModelParams::setRngSeedStr(newSeedStr);

        //std::seed_seq seed1{random_seed};
        //m_sRngEngine.seed(seed1);
        //std::stringstream ssSeed1;
        //ssSeed1 << strPrefix << random_seed;
        //ModelParams::setRngSeedStr(ssSeed1.str());
    }
    else
    {
        std::seed_seq seed2(seedStr.begin(), seedStr.end());
        m_sRngEngine.seed(seed2);
    }

    m_sbRngInitialised = true;
}


/**
 * @todo
 */
void EvoBeeModel::step()
{
    m_iStep++;
    cout << "Model step " << m_iStep << endl;
}
