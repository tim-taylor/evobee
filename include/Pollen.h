/**
 * @file
 *
 * Declaration of the Pollen class
 */

#ifndef _POLLEN_H
#define _POLLEN_H

#include <cassert>

class Flower;

/**
 * The Pollen class ...
 */
struct Pollen {
    Pollen(const Flower* pFlower, unsigned int _speciesId) : 
        pSource(pFlower),
        speciesId(_speciesId), 
        numLandings(0)
    {}

    // copy constructor
    Pollen(const Pollen& other) :
        pSource(other.pSource),
        speciesId(other.speciesId),
        numLandings(other.numLandings)
    {
        assert(false); // it is unlikely we should be here!
    }

    // move constructor
    Pollen(Pollen&& other) noexcept :
        pSource(other.pSource),
        speciesId(other.speciesId),
        numLandings(other.numLandings)
    {
    }

    // copy assignment operator
    Pollen& operator= (const Pollen& other)
    {
        assert(false); // it is unlikely we should be here!
        pSource = other.pSource;
        speciesId = other.speciesId;
        numLandings = other.numLandings;
        return *this;
    }

    // move assignment operator
    Pollen& operator= (Pollen&& other) noexcept
    {
        pSource = other.pSource;
        speciesId = other.speciesId;
        numLandings = other.numLandings;
        return *this;
    }    

    const Flower* pSource;
    unsigned int speciesId;
    int numLandings;
};

#endif /* _POLLEN_H */
