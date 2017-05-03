/**
 * @file
 *
 * Declaration of the Visualiser class
 */

#ifndef _VISUALISER_H
#define _VISUALISER_H

class SDL_Window;
class SDL_Renderer;

/**
 * The Visualiser class ...
 */
 class Visualiser {

public:
    Visualiser();
    ~Visualiser();

    int init();
    void update();

private:
    int           m_iScreenW;
    int           m_iScreenH;
    int           m_iPatchSize;
    SDL_Window*   m_pWindow;
    SDL_Renderer* m_pRenderer;
};

#endif /* _VISUALISER_H */
