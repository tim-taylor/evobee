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
    assert(ModelParams::initialised());

    // set up visualisation support as required
    m_bVis = ModelParams::getVisualisation();
    if (m_bVis) {
        m_Visualiser.init();
    }
    m_iVisUpdatePeriod = ModelParams::getVisUpdatePeriod();

    // initialise logging period
    m_iLogUpdatePeriod = ModelParams::getLogUpdatePeriod();

    // record run configuration details in log file
    if (ModelParams::getLogging())
    {
        m_Logger.logExptSetup();
    }
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
 * Run the model for the requested number of generations, using the requested
 * stopping criterion for each generation, as specified in the model parameters.
 * Also call logger and visualisation code as and when needed.
 */
void EvoBeeExperiment::run()
{
    for (int gen = 0; gen < ModelParams::getSimTerminationNumGens(); ++gen)
    {
        // if this is not the first generation, we need to reinitialise
        // all plants and pollinators based upon which plants got pollinated
        // in the previous generation. These details will be logged if
        // necessary.
        // Also reset the visualisation if needed.
        if (gen > 0)
        {
            m_Model.initialiseNewGeneration();
            if (m_bVis)
            {
                m_Visualiser.reset();
            }
        }

        bool endOfGen = false;
        bool bContinue = true;
        int step = 0;

        do
        {
            // perform the next simulation step
            m_Model.step();

            // perform logging if required
            if (ModelParams::getLogging() && (step % m_iLogUpdatePeriod == 0))
            {
                // We perform logging in a different thread, so that it can
                // continue in parallel with everything else until the next
                // time that the logging functionality is called.
                //
                // If the thread is still busy from a previous logger call,
                // wait until it is finished before calling the logger again.
                if (m_threadLog.joinable())
                {
                    m_threadLog.join();
                }
                m_threadLog = std::thread(&Logger::update, m_Logger);
            }

            // perform visualisation if required
            if ((m_bVis) && (step % m_iVisUpdatePeriod == 0)) 
            {
                bContinue = m_Visualiser.update();
                if (!bContinue)
                {
                    break; // may want to ask the user for confirmation first?
                }
            }

            // this step is now finished, so ...
            // ... advance step count
            ++step;
            // ... and check whether the current generation is now complete
            switch (ModelParams::getGenTerminationType())
            {
                case GenTerminationType::NUM_SIM_STEPS:
                {
                    if (step >= ModelParams::getGenTerminationIntParam())
                    {
                        endOfGen = true;
                    }
                    break;
                }
                case GenTerminationType::NUM_POLLINATOR_STEPS:
                {
                    ///@todo - implement code for num-pollinator-steps termination type
                    break;
                }
                case GenTerminationType::POLLINATED_FRACTION:
                {
                    if (m_Model.getEnv().getPollinatedFrac() >= ModelParams::getGenTerminationFloatParam())
                    {
                        endOfGen = true;
                    }
                    break;
                }
                default:
                {
                    throw std::runtime_error("Unexpected generation-termination-type encountered!");
                }
            }
        }
        while (!endOfGen);

        if (!bContinue)
        {
            break;
        }
    }
}
