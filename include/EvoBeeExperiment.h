/**
 * @file
 *
 * Declaration of the EvoBeeExperiment class
 */

#ifndef _EVOBEEEXPERIMENT_H
#define _EVOBEEEXPERIMENT_H

#include "EvoBeeModel.h"
#include "EventManager.h"
#include "Logger.h"
#include "Visualiser.h"

/**
 * The EvoBeeExperiment class ...
 */
class EvoBeeExperiment {

public:
    EvoBeeExperiment();
    ~EvoBeeExperiment() {}

    void run();

    EvoBeeModel         m_Model;
    EventManager        m_EventManager;
    Logger              m_Logger;
    Visualiser          m_Visualiser;

private:
    bool                m_bVis;
    int                 m_iVisUpdatePeriod;
};

#endif /* _EVOBEEEXPERIMENT_H */
