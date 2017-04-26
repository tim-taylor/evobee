/**
 * @file EvoBeeExperiment.cpp
 *
 * A desciption of the EvoBeeExperiment.cpp file...
 */

#include "ModelParams.h"
//#include "ExecutionConrtol.h"
#include "EventManager.h"
#include "Logger.h"
#include "Visualiser.h"
#include "EvoBeeExperiment.h"

EvoBeeExperiment::EvoBeeExperiment() :
//    m_ExptContoller(),
    m_EventManager(),
    m_Logger(),
    m_Visualiser()
{
    if (ModelParams::getVisualisation()) {
        m_Visualiser.init();
    }
}

/**
@todo
*/
void EvoBeeExperiment::run() {
    // this should be delegated to ExecutionController? - or maybe don't have that?

    // iterate increments in model and logger, and in visualiser if needed,
    // until halting criteria met
}
