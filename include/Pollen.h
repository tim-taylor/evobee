/**
 * @file
 *
 * Declaration of the Pollen class
 */

#ifndef _POLLEN_H
#define _POLLEN_H

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

    const Flower* pSource;
    unsigned int speciesId;
    int numLandings;
};

#endif /* _POLLEN_H */
