/**
 * @file
 *
 * Implementation of the EvoBeeExperiment class
 */

#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "EventManager.h"
#include "Logger.h"
#include "Visualiser.h"
#include "EvoBeeExperiment.h"


EvoBeeExperiment::EvoBeeExperiment() :
    m_Model(),
    m_EventManager(),
    m_Logger(),
    m_Visualiser(&m_Model)
{
    m_bVis = ModelParams::getVisualisation();
    if (m_bVis) {
        m_Visualiser.init();
    }
    m_iVisUpdatePeriod = ModelParams::getVisUpdatePeriod();

    ///@todo
    //m_Logger.logExptSetup(); // record param vals etc in log file
}

/**
@todo
*/
void EvoBeeExperiment::run() {
    // this should be delegated to ExecutionController? - or maybe don't have that?

    // iterate increments in model and logger, and in visualiser if needed,
    // until halting criteria met
    for (int step = 0; step < 150; ++step) 
    {
        m_Model.step();
        m_Logger.update(); // question: should this run in a different thread?
        if ((m_bVis) && (step % m_iVisUpdatePeriod == 0)) 
        {
            m_Visualiser.update(); // question: should this run in a different thread?
            // also, might want to be able to define a vis update interval in params, e.g.
            // update vis once very 10 steps
            // FROM SDL2 Documentation:
            // "NOTE: You should not expect to be able to create a window, render, or 
            // receive events on any thread other than the main one."
            // https://wiki.libsdl.org/CategoryThread
        }
    }
}
