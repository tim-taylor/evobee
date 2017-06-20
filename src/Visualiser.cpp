/**
 * @file
 *
 * Implementation of the Visualiser class
 */

#include <SDL.h>
#include <iostream>
#include <algorithm> // for min and max
#include <cassert>
#include "SDL2_gfxPrimitives.h"
#include "sdl_tools.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "Environment.h"
#include "Pollinator.h"
#include "Patch.h"
#include "Colour.h"
#include "Visualiser.h"


Visualiser::Visualiser(EvoBeeModel* pModel) :
    m_iScreenW(640),
    m_iScreenH(480),
    m_iPatchSize(10),
    m_fZoomLevel(1.0),
    m_iScreenOffsetX(0),
    m_iScreenOffsetY(0),
    m_iDelayMsPerFrame(0),
    m_pWindow(nullptr),
    m_pRenderer(nullptr),
    m_pModel(pModel)
{
    assert(ModelParams::initialised());
    m_bShowTrails = ModelParams::getVisPollinatorTrails();
}


Visualiser::~Visualiser()
{
    sdl_cleanup(m_pRenderer, m_pWindow);
    SDL_Quit();
}


int Visualiser::init()
{
    //std::cout << "In vis::init" << std::endl;

    // Initialise SDL (test)

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
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
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
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
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Make scaled rendering look smoother
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    return 0;
}

// Utility functions to convert Environment coordinates to screen coordinates

// -- first, the int versions
std::int16_t Visualiser::getScreenCoordFromIntX(int x)
{
    return m_iScreenOffsetX + (x * m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromIntY(int y)
{
    return m_iScreenOffsetY + (y * m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromIntWithAbsOffsetX(int x, int absOffset)
{
    return m_iScreenOffsetX + (x * m_iPatchSize) + absOffset;    
}

std::int16_t Visualiser::getScreenCoordFromIntWithAbsOffsetY(int y, int absOffset)
{
    return m_iScreenOffsetY + (y * m_iPatchSize) + absOffset;    
}

std::int16_t Visualiser::getScreenCoordFromIntWithRelOffsetX(int x, float relOffset)
{
    return m_iScreenOffsetX + (((float)x + relOffset) * (float)m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromIntWithRelOffsetY(int y, float relOffset)
{
    return m_iScreenOffsetY + (((float)y + relOffset) * (float)m_iPatchSize);
}

// - now, the float versions
std::int16_t Visualiser::getScreenCoordFromFloatX(float x)
{
    return m_iScreenOffsetX + (x * m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromFloatY(float y)
{
    return m_iScreenOffsetY + (y * m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromFloatWithAbsOffsetX(float x, int absOffset)
{
    return m_iScreenOffsetX + (x * m_iPatchSize) + absOffset;    
}

std::int16_t Visualiser::getScreenCoordFromFloatWithAbsOffsetY(float y, int absOffset)
{
    return m_iScreenOffsetY + (y * m_iPatchSize) + absOffset;    
}

std::int16_t Visualiser::getScreenCoordFromFloatWithRelOffsetX(float x, float relOffset)
{
    return m_iScreenOffsetX + ((x + relOffset) * (float)m_iPatchSize);
}

std::int16_t Visualiser::getScreenCoordFromFloatWithRelOffsetY(float y, float relOffset)
{
    return m_iScreenOffsetY + ((y + relOffset) * (float)m_iPatchSize);
}

std::int16_t Visualiser::getScreenLength(float len)
{
    return (std::int16_t)(len * (float)m_iPatchSize);
}


void Visualiser::update() {
    SDL_Event e;
    bool quit = false;

    /****/
    m_fZoomLevel += 0.01;
    m_iScreenOffsetX -= 1;
    m_iScreenOffsetY -= 1;
    m_iDelayMsPerFrame = 0;    
    SDL_RenderSetScale(m_pRenderer, m_fZoomLevel, m_fZoomLevel);
    //SDL_RenderSetLogicalSize(m_pRenderer, iZoomW, iZoomH);
    /****/  

    SDL_SetRenderDrawColor(m_pRenderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderClear(m_pRenderer);

    // render patches first
    auto patches = m_pModel->getEnv().getPatches();
    for (Patch& p : patches)
    {
        // render patches
        const Colour::RGB & c = Colour::getRgbFromMarkerPoint(p.getBackgroundMarkerPoint());
        const iPos & pos = p.getPosition();
        boxRGBA(
            m_pRenderer,
            getScreenCoordFromIntX(pos.x),
            getScreenCoordFromIntY(pos.y),
            getScreenCoordFromIntWithAbsOffsetX(pos.x+1, -1),
            getScreenCoordFromIntWithAbsOffsetY(pos.y+1, -1),
            c.r, c.g, c.b, 255
        );
    }

    // render flowers
    for (Patch& p : patches)
    {
        if (p.hasFloweringPlants())
        {
            PlantVector & fplants = p.getFloweringPlants();
            for (FloweringPlant & fplant : fplants)
            {                
                ///@todo deal with multiple flowers on a plant?

                Flower* pFlower = fplant.getFlower(0);
                const fPos & flwrpos = pFlower->getPosition();
                const Colour::RGB & c = Colour::getRgbFromMarkerPoint(pFlower->getMarkerPoint());

                if (pFlower->pollinated())
                {
                    // draw a pollinated flower
                    boxRGBA(
                        m_pRenderer,
                        getScreenCoordFromFloatX(flwrpos.x),
                        getScreenCoordFromFloatY(flwrpos.y),
                        getScreenCoordFromFloatWithAbsOffsetX(flwrpos.x+1.0, -1),
                        getScreenCoordFromFloatWithAbsOffsetY(flwrpos.y+1.0, -1),
                        150, 150, 150, 255
                    );
                }
                else
                {
                    // draw an unpollinated flower
                    filledCircleRGBA(
                        m_pRenderer,
                        getScreenCoordFromFloatX(flwrpos.x+0.5),
                        getScreenCoordFromFloatY(flwrpos.y+0.5),
                        getScreenLength(0.5),
                        c.r, c.g, c.b, 255
                    );
                }
            }
        }
    }

    // render hives
    HivePtrVector & hives = m_pModel->getEnv().getHives();
    for (auto& pHive : hives)
    {
        const fPos & hivepos = pHive->getPosition();
        boxRGBA(
            m_pRenderer,
            getScreenCoordFromFloatX(hivepos.x),
            getScreenCoordFromFloatY(hivepos.y),
            getScreenCoordFromFloatWithAbsOffsetX(hivepos.x+1.0, -1),
            getScreenCoordFromFloatWithAbsOffsetY(hivepos.y+1.0, -1),
            0, 0, 0, 255 ///@todo for now Hive visualisation colour is hard-coded as black
        );

        // now render each pollinator in the hive
        int numP = pHive->getNumPollinators();
        for (int i=0; i<numP; ++i)
        {
            Pollinator* p = pHive->getPollinator(i);
            const fPos& ppos = p->getPosition();

            boxRGBA(
                m_pRenderer,
                getScreenCoordFromFloatX(ppos.x),
                getScreenCoordFromFloatY(ppos.y),
                getScreenCoordFromFloatWithAbsOffsetX(ppos.x+1.0, -1),
                getScreenCoordFromFloatWithAbsOffsetY(ppos.y+1.0, -1),
                255, 255, 255, 255 ///@todo for now Hive visualisation colour is hard-coded as white
            );          

            if (m_bShowTrails)
            {
                unsigned int id = p->getId();
                unsigned int maxid = Pollinator::getMaxIdIssued();
                Uint8 r = 100 + ((150 * id) / maxid);
                Uint8 g = 250 - ((150 * id) / maxid);
                Uint8 b = r;

                // check if we already have a history record for this pollinator
                auto itr = m_PollMoveHists.find(id);
                if (itr == m_PollMoveHists.end())
                {
                    // no record found, so create a new one
                    m_PollMoveHists[id] = std::vector<fPos>{ ppos };
                    itr = m_PollMoveHists.find(id);
                }
                else
                {
                    // existing record found, so add current position to existing record
                    itr->second.push_back(ppos);
                }

                // if we have more than one previous position, plot the trail as a series
                // of lines between successive points in the history of previous positions
                const std::vector<fPos>& posHist = itr->second;
                if (posHist.size() > 1)
                {
                    auto prevPosItr = posHist.begin();
                    for (auto curPosItr = prevPosItr + 1; curPosItr != posHist.end(); ++curPosItr)
                    {
                        lineRGBA(
                            m_pRenderer,
                            getScreenCoordFromFloatWithRelOffsetX(prevPosItr->x, 0.5),
                            getScreenCoordFromFloatWithRelOffsetY(prevPosItr->y, 0.5),
                            getScreenCoordFromFloatWithRelOffsetX(curPosItr->x, 0.5),
                            getScreenCoordFromFloatWithRelOffsetY(curPosItr->y, 0.5),
                            r, g, b, 200
                        );
                        prevPosItr = curPosItr;                         
                    }
                }
            }     
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
    if (m_iDelayMsPerFrame > 0)
    {
        SDL_Delay(m_iDelayMsPerFrame);
    }

}