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
    // set up visualisation support as required
    m_bVis = ModelParams::getVisualisation();
    if (m_bVis) {
        m_Visualiser.init();
    }
    m_iVisUpdatePeriod = ModelParams::getVisUpdatePeriod();

    // record run configuration details in log file
    m_Logger.logExptSetup(); 
}

/**
 *
 */
void EvoBeeExperiment::run() {
    // iterate increments in model and logger, and in visualiser if needed,
    // until halting criteria met
    for (int step = 0; step < ModelParams::getTerminationNumSteps(); ++step) 
    {
        m_Model.step();
        m_Logger.update(); // question: should this run in a different thread?
        if ((m_bVis) && (step % m_iVisUpdatePeriod == 0)) 
        {
            m_Visualiser.update();
        }
    }
}
