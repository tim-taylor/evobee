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
    if (ModelParams::logging())
    {
        m_Logger.logExptSetup();
    }
}


EvoBeeExperiment::~EvoBeeExperiment()
{
    // Wait for any threads to finish their work
    if (ModelParams::useLogThreads())
    {
        if (m_threadLog.joinable())
        {
            m_threadLog.join();
        }
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
        ////////////////////////
        // REPRODUCTION PHASE //
        ////////////////////////

        // if this is not the first generation, we need to reinitialise
        // all plants and pollinators based upon which plants got pollinated
        // in the previous generation. These details will be logged if
        // necessary. Also reset the visualisation if needed.
        if (gen > 0)
        {
            m_Model.initialiseNewGeneration();
            if (m_bVis)
            {
                m_Visualiser.reset();
            }
        }

        ////////////////////
        // FORAGING PHASE //
        ////////////////////

        bool endOfGen = false;
        bool bContinue = true;
        int step = 0;

        do
        {
            // perform the next simulation step
            m_Model.step();

            // perform logging of pollinators if required
            if (ModelParams::logging() && (step % m_iLogUpdatePeriod == 0))
            {
                if (ModelParams::logPollinatorsIntraPhaseFull())
                {
                    if (ModelParams::useLogThreads())
                    {
                        // If the thread is still busy from a previous logger call,
                        // wait until it is finished before calling the logger again.
                        if (m_threadLog.joinable())
                        {
                            m_threadLog.join();
                        }
                        m_threadLog = std::thread(&Logger::logPollinatorsIntraPhaseFull, m_Logger);
                    }
                    else
                    {
                        m_Logger.logPollinatorsIntraPhaseFull();
                    }
                }

                if (ModelParams::logFlowersIntraPhaseSummary())
                {
                    if (ModelParams::useLogThreads())
                    {
                        if (m_threadLog.joinable())
                        {
                            m_threadLog.join();
                        }
                        m_threadLog = std::thread(&Logger::logFlowersIntraPhaseSummary, m_Logger);
                    }
                    else
                    {
                        m_Logger.logFlowersIntraPhaseSummary();
                    }
                }
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
                case GenTerminationType::POLLINATED_FRACTION_ALL:
                {
                    if (m_Model.getEnv().getPollinatedFracAll() >= ModelParams::getGenTerminationFloatParam())
                    {
                        endOfGen = true;
                    }
                    break;
                }
                case GenTerminationType::POLLINATED_FRACTION_SPECIES1:
                {
                    if (m_Model.getEnv().getPollinatedFracSpecies1() >= ModelParams::getGenTerminationFloatParam())
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
        while (!endOfGen); // end of foraging phase

        //////////////////////////////////////
        // LOGGING AT END OF FORAGING PHASE //
        //////////////////////////////////////

        // perform logging of flowers at the end of the generation if required
        if (ModelParams::logging())
        {
            if (ModelParams::logFlowersInterPhaseFull())
            {
                if (ModelParams::useLogThreads())
                {
                    if (m_threadLog.joinable())
                    {
                        m_threadLog.join();
                    }
                    m_threadLog = std::thread(&Logger::logFlowersInterPhaseFull, m_Logger);
                }
                else
                {
                    m_Logger.logFlowersInterPhaseFull();
                }
            }
            if (ModelParams::logFlowersInterPhaseSummary())
            {
                if (ModelParams::useLogThreads())
                {
                    if (m_threadLog.joinable())
                    {
                        m_threadLog.join();
                    }
                    m_threadLog = std::thread(&Logger::logFlowersInterPhaseSummary, m_Logger);
                }
                else
                {
                    m_Logger.logFlowersInterPhaseSummary();
                }
            }
            if (ModelParams::logPollinatorsInterPhaseSummary())
            {
                if (ModelParams::useLogThreads())
                {
                    if (m_threadLog.joinable())
                    {
                        m_threadLog.join();
                    }
                    m_threadLog = std::thread(&Logger::logPollinatorsInterPhaseSummary, m_Logger);
                }
                else
                {
                    m_Logger.logPollinatorsInterPhaseSummary();
                }
            }
        }

        if (!bContinue)
        {
            break;
        }
    }

    /////////////////////////////////
    // FINAL TIDY UP AT END OF RUN //
    /////////////////////////////////

    // at end of run, transfer all log files to the final destitination directory
    // if one has been specified
    m_Logger.transferFilesToFinalDir();
}
