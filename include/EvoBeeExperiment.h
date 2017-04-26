#ifndef _EVOBEEEXPERIMENT_H
#define _EVOBEEEXPERIMENT_H

/*
//class EvoBeeModel;
//class ExecutionControl;
class EventManager;
class Logger;
class Visualiser;
*/

#include "EventManager.h"
#include "Logger.h"
#include "Visualiser.h"

/**
*/
class EvoBeeExperiment {

public:
    EvoBeeExperiment();
    ~EvoBeeExperiment() {}

    void run();

    //EvoBeeModel         m_Model;
    //ExecutionControl    m_ExptController;
    EventManager        m_EventManager;
    Logger              m_Logger;
    Visualiser          m_Visualiser;
};

#endif /* _EVOBEEEXPERIMENT_H */
