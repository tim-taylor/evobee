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
    //
    /*
    std::string imagePath = getResourcePath("test") + "hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr)
    {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr)
    {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 2; ++i)
    {
        //First clear the renderer
        SDL_RenderClear(ren);
        //Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);
        //Update the screen
        SDL_RenderPresent(ren);

        // TT - added this otherwise image doesn't show until after the first delay..
        //First clear the renderer
        SDL_RenderClear(ren);

        //Take a quick break after all that hard work
        SDL_Delay(1000);
    }
    */

    /*
    //Fill the surface white
    //Get window surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface( win );
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0x80, 0x00 ) );
    //Update the surface
    SDL_UpdateWindowSurface( win );
    SDL_Delay(3000);
    */

    //SDL_DestroyTexture(tex);

    return 0;
}


void Visualiser::update() {
    SDL_Event e;
    bool quit = false;

    //while (!quit)
    {
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

        SDL_SetRenderDrawColor(m_pRenderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(m_pRenderer);

        //static int step = 300; //@todo this is just for testing
        //step += 2; //@todo this is just for testing

        auto patches = m_pModel->getEnv().getPatches();
        for (Patch& p : patches)
        {

            auto c = Colour::getRgbFromMarkerPoint(p.getBackgroundMarkerPoint());
            auto x = p.getPosX();
            auto y = p.getPosY();
            boxRGBA(m_pRenderer,
                     x * m_iPatchSize,
                     y * m_iPatchSize,
                     (x + 1) * m_iPatchSize - 1, (y + 1) * m_iPatchSize - 1,
                     c.r, c.g, c.b, 255);

            if (p.hasFloweringPlants())
            {
                int offset = 0;
                auto fplants = p.getFloweringPlants();
                for (FloweringPlant fplant : fplants)
                {
                    ///@todo at present just using offsets rather than exact position in parch
                    auto c = Colour::getRgbFromMarkerPoint(fplant.getFlowerMarkerPoint());
                    filledCircleRGBA(m_pRenderer,
                        x * m_iPatchSize + offset, 
                        y * m_iPatchSize + offset,
                        m_iPatchSize/2,
                        c.r, c.g, c.b, 255);
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
            */
        }

        /*
        thickLineColor(ren, 0, 0, SCREEN_W, SCREEN_H, 20, 0xFF00FFFF);
        thickLineColor(ren, 0, SCREEN_H, SCREEN_W, 0, 20, 0xFF00FFFF);
        circleColor(ren, SCREEN_W / 2, SCREEN_H / 2, 33, 0xff00ff00);
        filledCircleColor(ren, SCREEN_W / 2, SCREEN_H / 2, 30, 0xff00ffcc);
        */

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
        //}
        
        //SDL_Delay(3000);
    }
}