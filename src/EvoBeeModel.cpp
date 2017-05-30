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
        ModelParams::setRngSeedStr(newSeedStr);
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
    ++m_iStep;
    cout << "Model step " << m_iStep << endl;

    ///@todo - Careful - we really need to shuffle order of all pollinators
    // possiblility would be for the Env to store a single vector of pointers to Pollinators
    // for ALL hives, and do a random shuffle on that

    // first allow all pollinators to update
    auto pollinators = m_Env.getAllPollinators();
    std::shuffle(pollinators.begin(), pollinators.end(), m_sRngEngine);
    for (Pollinator* pPol : pollinators)
    {
        // do stuff...
    }
    /*
    auto hives = m_Env.getHives();
    for (auto &pHive : hives)
    {
        int numP = pHive->getNumPollinators();
        for (int i=0; i<numP; ++i)
        {
            Pollinator* p = pHive->getPollinator(i);
            //@todo
        }
    }
    */

    ///@todo - what else required?    
}
