/**
 * @file
 *
 * Implementation of the Visualiser class
 */

#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm> // for min and max
#include <cassert>
#include "SDL3_gfxPrimitives.h"
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
    m_iPatchSize(20),
    m_fZoomLevel(1.0),
    m_iScreenOffsetX(0),
    m_iScreenOffsetY(0),
    m_bShowFlowers(true),
    m_bShowPollinators(true),
    m_bUpdate(true),
    m_pWindow(nullptr),
    m_pRenderer(nullptr),
    m_pModel(pModel)
{
    assert(ModelParams::initialised());
    m_bShowTrails = ModelParams::getVisPollinatorTrails();
    m_iDelayMsPerFrame = ModelParams::getVisDelayPerFrame();
}


Visualiser::~Visualiser()
{
    sdl_cleanup(m_pRenderer, m_pWindow);
    SDL_Quit();
}


int Visualiser::init()
{
    // Initialise SDL (test)
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //-------------------------------------------------------------------------
    // Calculate appropriate width and height for window
    //-------------------------------------------------------------------------

    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    if (displays == nullptr || num_displays < 1)
    {
        std::cerr << "SDL_Init Error: no displays found" << std::endl;
        return 1;
    }
    int displayInstanceID = displays[0];
    SDL_free(displays);

    const SDL_DisplayMode* pDM = SDL_GetDesktopDisplayMode(displayInstanceID);
    if (pDM == nullptr) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    m_iScreenW = (int)(ModelParams::getMaxScreenFracW() * (float)(pDM->w));
    m_iScreenH = (int)(ModelParams::getMaxScreenFracH() * (float)(pDM->h));

    float fScreenToEnvRatio = std::min<float>(
        (float)m_iScreenW / (float)ModelParams::getEnvSizeX(),
        (float)m_iScreenH / (float)ModelParams::getEnvSizeY()
    );

    m_fZoomLevel = fScreenToEnvRatio / (float)m_iPatchSize;

    // having calculated the zoom level, now readjust screen width and height so that
    // it is exactly the right size for the specified environment size at this zoom level
    m_iScreenW = m_fZoomLevel * (float)m_iPatchSize * (float)ModelParams::getEnvSizeX();
    m_iScreenH = m_fZoomLevel * (float)m_iPatchSize * (float)ModelParams::getEnvSizeY();

    //-------------------------------------------------------------------------
    // Create Window
    //-------------------------------------------------------------------------

    m_pWindow = SDL_CreateWindow(
        "EvoBee",
        //SDL_WINDOWPOS_CENTERED,
        //SDL_WINDOWPOS_CENTERED,
        m_iScreenW, m_iScreenH,
        SDL_WINDOW_OPENGL);
    if (m_pWindow == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowPosition(m_pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    //-------------------------------------------------------------------------
    // Create Renderer
    //-------------------------------------------------------------------------

    m_pRenderer = SDL_CreateRenderer(
        m_pWindow,
        nullptr
        //-1,
        //0 /*SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC*/
        );
    if (m_pRenderer == nullptr)
    {
        sdl_cleanup(m_pWindow);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Make scaled rendering look smoother
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    return 0;
}


bool Visualiser::update()
{
    bool bContinue = true;

    if (!m_bUpdate) {
        bContinue = checkUserInteraction();
        return bContinue;
    }


    SDL_SetRenderScale(m_pRenderer, m_fZoomLevel, m_fZoomLevel);
    SDL_SetRenderDrawColor(m_pRenderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderClear(m_pRenderer);

    // render patches first
    PatchVector& patches = m_pModel->getEnv().getPatches();
    for (Patch& p : patches)
    {
        // render patches
        //const Colour::RGB & pc = Colour::getRgbFromMarkerPoint(p.getBackgroundMarkerPoint());
        const Colour::RGB & pc = Colour::getRgbFromMarkerPoint(400);
        const iPos & pos = p.getPosition();
        boxRGBA(
            m_pRenderer,
            getScreenCoordFromIntX(pos.x),
            getScreenCoordFromIntY(pos.y),
            getScreenCoordFromIntWithAbsOffsetX(pos.x+1, -1),
            getScreenCoordFromIntWithAbsOffsetY(pos.y+1, -1),
            pc.r, pc.g, pc.b, 50
        );
    }

    // render flowers
    Colour c;

    if (m_bShowFlowers)
    {
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

                    //const Colour::RGB & c = Colour::getRgbFromMarkerPoint(pFlower->getCharacteristicWavelength());

                    ///@todo prob want to replace the hardcoded limits in the following line with variables...
                    Colour::setColourInRange(pFlower->getCharacteristicWavelength(), 300, 650, c);

                    if (pFlower->pollinated())
                    {
                        // draw a pollinated flower
                        boxRGBA(
                            m_pRenderer,
                            getScreenCoordFromFloatWithRelOffsetX(flwrpos.x, 0.15),
                            getScreenCoordFromFloatWithRelOffsetY(flwrpos.y, 0.15),
                            getScreenCoordFromFloatWithRelOffsetX(flwrpos.x, 0.85),
                            getScreenCoordFromFloatWithRelOffsetY(flwrpos.y, 0.85),
                            c.getR(), c.getG(), c.getB(), 200
                        );
                    }
                    else
                    {
                        // draw an unpollinated flower
                        boxRGBA(
                            m_pRenderer,
                            getScreenCoordFromFloatWithRelOffsetX(flwrpos.x, 0.30),
                            getScreenCoordFromFloatWithRelOffsetY(flwrpos.y, 0.30),
                            getScreenCoordFromFloatWithRelOffsetX(flwrpos.x, 0.70),
                            getScreenCoordFromFloatWithRelOffsetY(flwrpos.y, 0.70),
                            c.getR(), c.getG(), c.getB(), 255
                        );
                    }
                }
            }
        }
    }

    // render hives and pollinators
    if (m_bShowPollinators)
    {
        HivePtrVector & hives = m_pModel->getEnv().getHives();
        for (auto& pHive : hives)
        {
            // draw the hive
            const fPos & hivepos = pHive->getPosition();
            boxRGBA(
                m_pRenderer,
                getScreenCoordFromFloatWithRelOffsetX(hivepos.x, 0.10),
                getScreenCoordFromFloatWithRelOffsetY(hivepos.y, 0.10),
                getScreenCoordFromFloatWithRelOffsetX(hivepos.x, 0.90),
                getScreenCoordFromFloatWithRelOffsetY(hivepos.y, 0.90),
                0, 0, 0, 255 ///@todo for now Hive visualisation colour is hard-coded as black
            );

            // now render each pollinator in the hive
            int numP = pHive->getNumPollinators();
            for (int i=0; i<numP; ++i)
            {
                Pollinator* p = pHive->getPollinator(i);
                const fPos& ppos = p->getPosition();
                MarkerPoint mpTarget = p->getTargetWavelength();
                const Colour::RGB & pc = Colour::getRgbFromMarkerPoint(mpTarget);

                // draw the pollinator
                boxRGBA(
                    m_pRenderer,
                    getScreenCoordFromFloatWithRelOffsetX(ppos.x, 0.35),
                    getScreenCoordFromFloatWithRelOffsetY(ppos.y, 0.35),
                    getScreenCoordFromFloatWithRelOffsetX(ppos.x, 0.65),
                    getScreenCoordFromFloatWithRelOffsetY(ppos.y, 0.65),
                    pc.r, pc.g, pc.b, 200
                    //255, 255, 255, 255 ///@todo for now pollinator visualisation colour is hard-coded as white
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
                        auto curPosItr = posHist.begin();
                        ++curPosItr;
                        for (; curPosItr != posHist.end(); ++curPosItr, ++prevPosItr)
                        {
                            lineRGBA(
                                m_pRenderer,
                                getScreenCoordFromFloatWithRelOffsetX(prevPosItr->x, 0.5),
                                getScreenCoordFromFloatWithRelOffsetY(prevPosItr->y, 0.5),
                                getScreenCoordFromFloatWithRelOffsetX(curPosItr->x, 0.5),
                                getScreenCoordFromFloatWithRelOffsetY(curPosItr->y, 0.5),
                                r, g, b, 200
                            );
                        }
                    }
                }
            }
        }
    }


    // Now everything is prepared, render the entire scence
    SDL_RenderPresent(m_pRenderer);

    bContinue = checkUserInteraction();

    if (m_iDelayMsPerFrame > 0)
    {
        SDL_Delay(m_iDelayMsPerFrame);
    }

    return bContinue;
}


bool Visualiser::checkUserInteraction()
{
    // Deal with user interactivity
    bool pause = false;
    bool bContinue = true;

    do
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                bContinue = false;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN)
            {
                switch (e.key.key)
                {
                    case SDLK_X:
                    {
                        m_fZoomLevel = std::min(m_fZoomLevel + 0.1, 10.0);
                        break;
                    }
                    case SDLK_Z:
                    {
                        m_fZoomLevel = std::max(m_fZoomLevel - 0.1, 0.1);
                        break;
                    }
                    case SDLK_LEFT:
                    {
                        m_iScreenOffsetX -= 5;
                        break;
                    }
                    case SDLK_RIGHT:
                    {
                        m_iScreenOffsetX += 5;
                        break;
                    }
                    case SDLK_UP:
                    {
                        m_iScreenOffsetY -= 5;
                        break;
                    }
                    case SDLK_DOWN:
                    {
                        m_iScreenOffsetY += 5;
                        break;
                    }
                    case SDLK_P:
                    {
                        pause = !pause;
                        break;
                    }
                    case SDLK_F:
                    {
                        m_bShowFlowers = !m_bShowFlowers;
                        break;
                    }
                    case SDLK_B:
                    {
                        m_bShowPollinators = !m_bShowPollinators;
                        break;
                    }
                    case SDLK_T:
                    {
                        m_bShowTrails = !m_bShowTrails;
                        break;
                    }
                    case SDLK_U:
                    {
                        m_bUpdate = !m_bUpdate;
                        break;
                    }
                }
            }
        }
    } while (pause);

    return bContinue;
}


void Visualiser::reset()
{
    // clear the vector of records of previous positions for each pollinator
    for (auto& item : m_PollMoveHists)
    {
        item.second.clear();
    }
}


/////////////////////////
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
