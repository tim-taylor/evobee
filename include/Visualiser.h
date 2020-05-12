/**
 * @file
 *
 * Declaration of the Visualiser class
 */

#ifndef _VISUALISER_H
#define _VISUALISER_H

#include <map>
#include <vector>
#include <cinttypes>
#include "Position.h"

class SDL_Window;
class SDL_Renderer;
class EvoBeeModel;

/**
 * The Visualiser class ...
 */
class Visualiser {

public:
    Visualiser(EvoBeeModel* pModel);
    ~Visualiser();

    /**
     * Initialise visualisation code
     */
    int init();

    /**
     * Run a single frame update of the visualiser, and deal with any user interactions
     *
     * @return true if all good, false if user has pressed the Quit key
     */
    bool update();

    /**
     * Reset the state of the visualiser for the start of a new generation.
     * Very little needs to be done here, just clear the vector of pollinator trails.
     */
    void reset();

private:
    // Utility functions to map from Environment coordinates to screen coordinates
    // taking into account current scaling and offset values. These are separated
    // out into different functions to deal with different types of offsets, rather
    // than having a single function that can deal with all cases, for efficiency
    // reasons, as these functions get called many times during each frame of the
    // visualisation code.

    // There are int and float versions of most of these, so that we have more control
    // over when, exactly, any float to int conversions happen, as this can affect the
    // results

    // first the int versions
    inline std::int16_t getScreenCoordFromIntX(int x);
    inline std::int16_t getScreenCoordFromIntY(int y);

    inline std::int16_t getScreenCoordFromIntWithAbsOffsetX(int x, int absOff);
    inline std::int16_t getScreenCoordFromIntWithAbsOffsetY(int y, int absOff);

    inline std::int16_t getScreenCoordFromIntWithRelOffsetX(int x, float relOff);
    inline std::int16_t getScreenCoordFromIntWithRelOffsetY(int y, float relOff);

    // and now the float versions
    inline std::int16_t getScreenCoordFromFloatX(float x);
    inline std::int16_t getScreenCoordFromFloatY(float y);

    inline std::int16_t getScreenCoordFromFloatWithAbsOffsetX(float x, int absOff);
    inline std::int16_t getScreenCoordFromFloatWithAbsOffsetY(float y, int absOff);

    inline std::int16_t getScreenCoordFromFloatWithRelOffsetX(float x, float relOff);
    inline std::int16_t getScreenCoordFromFloatWithRelOffsetY(float y, float relOff);

    inline std::int16_t getScreenLength(float len);

    bool checkUserInteraction();


    // private data members
    int     m_iScreenW;
    int     m_iScreenH;
    int     m_iPatchSize; /** determines the fundemental resolution for each patch
                           *  i.e. the number of pixels width and height used to draw
                           *  each patch, before any zoom scaling is applied.
                           */
    float   m_fZoomLevel;
    int     m_iScreenOffsetX;
    int     m_iScreenOffsetY;

    int     m_iDelayMsPerFrame;

    bool    m_bShowFlowers;
    bool    m_bShowPollinators;
    bool    m_bShowTrails;
    bool    m_bUpdate;
    std::map<unsigned int, std::vector<fPos>> m_PollMoveHists; /** map of Pollinator ids with the movement
                                                                *  history of that pollinator */
    SDL_Window*   m_pWindow;
    SDL_Renderer* m_pRenderer;
    EvoBeeModel*  m_pModel;
};

#endif /* _VISUALISER_H */
