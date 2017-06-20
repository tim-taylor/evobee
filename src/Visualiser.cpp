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
            (pos.x + 1) * m_iPatchSize - 1,
            (pos.y + 1) * m_iPatchSize - 1,
            c.r, c.g, c.b, 255
        );

        // render flowers
        if (p.hasFloweringPlants())
        {
            int offset = 0;
            auto fplants = p.getFloweringPlants();
            for (FloweringPlant& fplant : fplants)
            {
                ///@todo at present just using offsets rather than exact position 
                /// of each flower in the patch
                
                ///@todo deal with multiple flowers on a plant?

                Flower* pFlower = fplant.getFlower(0);
                auto c = Colour::getRgbFromMarkerPoint(pFlower->getMarkerPoint());

                if (pFlower->pollinated())
                {
                    // draw a pollinated flower
                    boxRGBA(
                        m_pRenderer,
                        pos.x * m_iPatchSize + offset,
                        pos.y * m_iPatchSize + offset,
                        (pos.x + 1) * m_iPatchSize - 1 + offset, 
                        (pos.y + 1) * m_iPatchSize - 1 + offset,
                        150, 150, 150, 255
                    );
                }
                else
                {
                    // draw an unpollinated flower
                    filledCircleRGBA(
                        m_pRenderer,
                        pos.x * m_iPatchSize + offset, 
                        pos.y * m_iPatchSize + offset,
                        m_iPatchSize/2,
                        c.r, c.g, c.b, 255
                    );
                }

                ++offset;
            }
        }
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
            (hivepos.x + 1) * m_iPatchSize - 1,
            (hivepos.y + 1) * m_iPatchSize - 1,
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
                ppos.x * m_iPatchSize,
                ppos.y * m_iPatchSize,
                (ppos.x + 1) * m_iPatchSize - 1,
                (ppos.y + 1) * m_iPatchSize - 1,
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
                            prevPosItr->x * m_iPatchSize + (0.5 * m_iPatchSize),
                            prevPosItr->y * m_iPatchSize + (0.5 * m_iPatchSize),
                            curPosItr->x * m_iPatchSize + (0.5 * m_iPatchSize),
                            curPosItr->y * m_iPatchSize + (0.5 * m_iPatchSize),
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
    //SDL_Delay(3000);

}