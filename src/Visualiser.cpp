/**
 * @file
 *
 * Implementation of the Visualiser class
 */

#include <SDL.h>
#include <iostream>
#include <algorithm> // for min and max
#include "SDL2_gfxPrimitives.h"
#include "sdl_tools.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "Environment.h"
#include "Pollinator.h"
#include "Patch.h"
#include "Colour.h"
#include "Visualiser.h"

using std::cout;
using std::cerr;

Visualiser::Visualiser(EvoBeeModel* pModel) :
    m_iScreenW(640),
    m_iScreenH(480),
    m_iPatchSize(10),
    m_pWindow(nullptr),
    m_pRenderer(nullptr),
    m_pModel(pModel)
{
}


Visualiser::~Visualiser()
{
    sdl_cleanup(m_pRenderer, m_pWindow);
    SDL_Quit();
}


int Visualiser::init()
{
    std::cout << "In vis::init" << std::endl;

    // Initialise SDL (test)

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //-------------------------------------------------------------------------
    // Calculate appropriate width and height for window
    //-------------------------------------------------------------------------

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }   

    m_iScreenW = (int)(ModelParams::getMaxScreenFracW() * (float)dm.w);
    m_iScreenH = (int)(ModelParams::getMaxScreenFracH() * (float)dm.h);

    m_iPatchSize = std::max(1, std::min(
        m_iScreenW / ModelParams::getEnvSizeX(),
        m_iScreenH / ModelParams::getEnvSizeY()
    )
    );

    m_iScreenW = m_iPatchSize * ModelParams::getEnvSizeX();
    m_iScreenH = m_iPatchSize * ModelParams::getEnvSizeY();
    // NB The SDL_RenderSetScale() function might come in handy, e.g. to allow
    // user to zoom in and out of display easily


    //-------------------------------------------------------------------------
    // Create Window
    //-------------------------------------------------------------------------

    m_pWindow = SDL_CreateWindow(
        "EvoBee",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_iScreenW, m_iScreenH,
        SDL_WINDOW_OPENGL);
    if (m_pWindow == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //-------------------------------------------------------------------------
    // Create Renderer
    //-------------------------------------------------------------------------

    m_pRenderer = SDL_CreateRenderer(
        m_pWindow,
        -1,
        0 /*SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC*/
        );
    if (m_pRenderer == nullptr)
    {
        sdl_cleanup(m_pWindow);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    return 0;
}


void Visualiser::update() {
    SDL_Event e;
    bool quit = false;

    SDL_SetRenderDrawColor(m_pRenderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderClear(m_pRenderer);

    auto patches = m_pModel->getEnv().getPatches();
    for (Patch& p : patches)
    {
        // render patches
        auto c = Colour::getRgbFromMarkerPoint(p.getBackgroundMarkerPoint());
        auto pos = p.getPosition();
        boxRGBA(
            m_pRenderer,
            pos.x * m_iPatchSize,
            pos.y * m_iPatchSize,
            (pos.x + 1) * m_iPatchSize - 1, (pos.y + 1) * m_iPatchSize - 1,
            c.r, c.g, c.b, 255
        );

        // render flowers
        if (p.hasFloweringPlants())
        {
            int offset = 0;
            auto fplants = p.getFloweringPlants();
            for (FloweringPlant& fplant : fplants)
            {
                ///@todo at present just using offsets rather than exact position in patch
                auto c = Colour::getRgbFromMarkerPoint(fplant.getFlowerMarkerPoint());
                filledCircleRGBA(
                    m_pRenderer,
                    pos.x * m_iPatchSize + offset, 
                    pos.y * m_iPatchSize + offset,
                    m_iPatchSize/2,
                    c.r, c.g, c.b, 255
                );
                ++offset;
            }
        }

        /*
        if (m_iPatchSize > 1)
        {
            rectangleRGBA(m_pRenderer,
                        x * m_iPatchSize,
                        y * m_iPatchSize,
                        (x + 1) * m_iPatchSize - 1, (y + 1) * m_iPatchSize - 1,
                        200, 200, 200, 255);
        }

        //thickLineColor(ren, 0, 0, SCREEN_W, SCREEN_H, 20, 0xFF00FFFF);
        //thickLineColor(ren, 0, SCREEN_H, SCREEN_W, 0, 20, 0xFF00FFFF);
        //circleColor(ren, SCREEN_W / 2, SCREEN_H / 2, 33, 0xff00ff00);
        //filledCircleColor(ren, SCREEN_W / 2, SCREEN_H / 2, 30, 0xff00ffcc);
                
        */
    }

    // render hives
    auto hives = m_pModel->getEnv().getHives();
    for (auto &pHive : hives)
    {
        auto hivepos = pHive->getPosition();
        boxRGBA(
            m_pRenderer,
            hivepos.x * m_iPatchSize,
            hivepos.y * m_iPatchSize,
            (hivepos.x + 1) * m_iPatchSize - 1, (hivepos.y + 1) * m_iPatchSize - 1,
            0, 0, 0, 255 ///@todo for now Hive visualisation colour is hard-coded as black
        );

        // now render each pollinator in the hive
        int numP = pHive->getNumPollinators();
        for (int i=0; i<numP; ++i)
        {
            //@todo
            Pollinator* p = pHive->getPollinator(i);
            auto ppos = p->getPosition();
            boxRGBA(
                m_pRenderer,
                ppos.x * m_iPatchSize,
                ppos.y * m_iPatchSize,
                (ppos.x + 1) * m_iPatchSize - 1, (ppos.y + 1) * m_iPatchSize - 1,
                255, 255, 255, 255 ///@todo for now Hive visualisation colour is hard-coded as white
            );            
        }
    }


    // Now everythin is prepared, render the entire scence
    SDL_RenderPresent(m_pRenderer);

    //while(!quit)
    //{
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        /*
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
        }
        */
    //}
    //SDL_Delay(3000);

}