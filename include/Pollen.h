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
    //Pollen() : pSource(nullptr), numLandings(0) {}
    Pollen(const Flower* pFlower) : pSource(pFlower), numLandings(0) {}

    const Flower* pSource;
    int numLandings;
};

#endif /* _POLLEN_H */
