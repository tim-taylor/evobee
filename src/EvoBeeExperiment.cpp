/**
 * @file
 *
 * Implementation of the EvoBeeExperiment class
 */

#include <thread>
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "EventManager.h"
#include "Logger.h"
#include "Visualiser.h"
#include "EvoBeeExperiment.h"


EvoBeeExperiment::EvoBeeExperiment() :
    m_Model(),
    m_EventManager(),
    m_Logger(&m_Model),
    m_Visualiser(&m_Model),
    m_threadLog()
{
    // set up visualisation support as required
    m_bVis = ModelParams::getVisualisation();
    if (m_bVis) {
        m_Visualiser.init();
    }
    m_iVisUpdatePeriod = ModelParams::getVisUpdatePeriod();

    // initialise logging period
    m_iLogUpdatePeriod = ModelParams::getLogUpdatePeriod();

    // record run configuration details in log file
    m_Logger.logExptSetup(); 
}


EvoBeeExperiment::~EvoBeeExperiment()
{
    // Wait for any threads to finish their work
    if (m_threadLog.joinable())
    {
        m_threadLog.join();
    }
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

        if (step % m_iLogUpdatePeriod == 0)
        {
            // if the thread is still busy from a previous logger call,
            // wait until it is finished before calling the logger again
            if (m_threadLog.joinable())
            {
                m_threadLog.join();
            }            
            m_threadLog = std::thread(&Logger::update, m_Logger);
        }

        if ((m_bVis) && (step % m_iVisUpdatePeriod == 0)) 
        {
            m_Visualiser.update();
        }
    }
}
