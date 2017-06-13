/**
 * @file
 *
 * Declaration of the Visualiser class
 */

#ifndef _VISUALISER_H
#define _VISUALISER_H

#include <map>
#include <vector>
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

    int init();
    void update();

private:
    int           m_iScreenW;
    int           m_iScreenH;
    int           m_iPatchSize;
    
    bool          m_bShowTrails;
    std::map<unsigned int, std::vector<fPos>> m_PollMoveHists; //< map of Pollinator ids with the movement
                                                               //< history of that pollinator

    SDL_Window*   m_pWindow;
    SDL_Renderer* m_pRenderer;
    EvoBeeModel*  m_pModel;
};

#endif /* _VISUALISER_H */
