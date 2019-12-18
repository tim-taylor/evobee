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
#include "HoneyBee.h"

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
    unsigned int testnum = ModelParams::getTestNumber();
    switch (testnum)
    {
    case 0:
        runStandardExperiment();
        break;
    case 1:
        runMarkerPointSimilarityTest();
        break;
    case 2:
        runMatchConfidenceTest();
        break;
    default:
        std::cerr << "Unknown test number " << testnum << " requested. Aborting." << std::endl;
        exit(1);
    }
}


void EvoBeeExperiment::runStandardExperiment()
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
                if (ModelParams::logPollinatorsIntraPhaseFull()) {
                    callLoggerMethod(&Logger::logPollinatorsIntraPhaseFull);
                }
                if (ModelParams::logFlowersIntraPhaseFull()) {
                    callLoggerMethod(&Logger::logFlowersIntraPhaseFull);
                }
                if (ModelParams::logFlowersIntraPhaseSummary()) {
                    callLoggerMethod(&Logger::logFlowersIntraPhaseSummary);
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
            if (ModelParams::logFlowersInterPhaseFull()) {
                callLoggerMethod(&Logger::logFlowersInterPhaseFull);
            }
            if (ModelParams::logFlowersInterPhaseSummary()) {
                callLoggerMethod(&Logger::logFlowersInterPhaseSummary);
            }
            if (ModelParams::logFlowerMPsInterPhaseSummary()) {
                callLoggerMethod(&Logger::logFlowerMPsInterPhaseSummary);
            }
            if (ModelParams::logPollinatorsInterPhaseFull()) {
                callLoggerMethod(&Logger::logPollinatorsInterPhaseFull);
            }
            if (ModelParams::logPollinatorsInterPhaseSummary()) {
                callLoggerMethod(&Logger::logPollinatorsInterPhaseSummary);
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
    if (ModelParams::logging())
    {
        m_Logger.transferFilesToFinalDir();
    }

}


void EvoBeeExperiment::runMarkerPointSimilarityTest()
{
    auto pPolConfig = ModelParams::getPollinatorConfigPtr("HoneyBee");
    auto hives = m_Model.getEnv().getHives();
    auto pHive = hives.at(0);
    for (MarkerPoint mp = 300; mp <= 650; mp+=10) {
        HoneyBee bee{*pPolConfig, (AbstractHive*)&(*pHive)};
        bee.setTargetMP(mp);
        for (MarkerPoint mp2 = 300; mp2 <= 650; mp2+=10) {
            ReflectanceInfo stimulus{mp2};
            bool matches = bee.matchesTargetMP(stimulus);
            std::cout << mp << "," << mp2 << "," << (matches ? "1" : "0") << std::endl;
        }
    }
    return;
}


void EvoBeeExperiment::runMatchConfidenceTest()
{
    auto pPolConfig = ModelParams::getPollinatorConfigPtr("HoneyBee");
    auto hives = m_Model.getEnv().getHives();
    auto pHive = hives.at(0);
    for (MarkerPoint mp = 300; mp <= 650; mp+=10) {
        HoneyBee bee{*pPolConfig, (AbstractHive*)&(*pHive)};
        bee.setTargetMP(mp);
        for (MarkerPoint mp2 = 300; mp2 <= 650; mp2+=10) {
            ReflectanceInfo stimulus{mp2};
            float confidence = bee.confidenceMatchesTarget(stimulus);
            std::cout << mp << "," << mp2 << "," << std::setprecision(5) << confidence << std::endl;
        }
    }
    return;
}


void EvoBeeExperiment::callLoggerMethod(void (Logger::*loggerMethod)())
{
    if (ModelParams::useLogThreads())
    {
        if (m_threadLog.joinable())
        {
            m_threadLog.join();
        }
        m_threadLog = std::thread(loggerMethod, m_Logger);
    }
    else
    {
        (m_Logger.*loggerMethod)();
    }
}
