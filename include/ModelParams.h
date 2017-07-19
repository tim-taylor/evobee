/**
 * @file
 *
 * Declaration of the ModelParams class
 */

#ifndef _MODELPARAMS_H
#define _MODELPARAMS_H

#include <vector>
#include <string>
#include "json.hpp"
#include "HiveConfig.h"
#include "PlantTypeDistributionConfig.h"
#include "PlantTypeConfig.h"
#include "PollinatorConfig.h"
#include "ReflectanceInfo.h"
#include "EvoBeeExperiment.h"


/**
 * The ModelParams Class holds a record of all model parameters. 
 *
 * All methods and variables of this class are static and can therefore be accessed
 * by any other object in the system.
 */
class ModelParams {

public:
    static void setEnvSize(int x, int y);
    static void setEnvSizeX(int x);
    static void setEnvSizeY(int y);
    static void setEnvDefaultAmbientTemp(float temp);
    static void setEnvBackgroundReflectanceMP(MarkerPoint mp);
    static void setReproGlobalDensityConstrained(bool constrained);
    static void setReproGlobalDensityMax(float dmax);
    static void setVisualisation(bool vis);
    static void setMaxScreenFrac(float f);
    static void setMaxScreenFracW(float fw);
    static void setMaxScreenFracH(float fh);
    static void setVisPollinatorTrails(bool show);
    static void setVisUpdatePeriod(int p);
    static void setVisDelayPerFrame(int delayMs);
    static void setLogging(bool logging);
    static void setLogFlags(const std::string& flags);
    static void setLogUpdatePeriod(int p);
    static void setInitialised();
    static void setSimTerminationNumGens(int gens);
    static void setGenTerminationType(const std::string& typestr);
    static void setGenTerminationParam(int p);
    static void setGenTerminationParam(float p);
    static void setGenTerminationIntParam(int p);
    static void setGenTerminationFloatParam(float p);
    static void setRngSeedStr(const std::string& seed, bool bRewriteJsonEntry = false);
    static void setLogDir(const std::string& dir);
    static void setLogRunName(const std::string& name);

    static void addHiveConfig(HiveConfig& hc);
    static void addPlantTypeDistributionConfig(PlantTypeDistributionConfig& pc);
    static void addPlantTypeConfig(PlantTypeConfig& pt);
    static void addPollinatorConfig(PollinatorConfig& pt);

    /// check consistency of parameters once the whole config file has been processed
    static void checkConsistency();

    static int   getEnvSizeX() {return m_iEnvSizeX;}
    static int   getEnvSizeY() {return m_iEnvSizeY;}
    static float getEnvDefaultAmbientTemp() {return m_fEnvDefaultAmbientTemp;}
    static bool  getReproGlobalDensityConstrained() {return m_bReproGlobalDensityContrained;}
    static float getReproGlobalDensityMax() {return m_fReproGlobalDensityMax;}
    static int   getNumPatches() {return m_iEnvSizeX * m_iEnvSizeY;}
    static bool  getVisualisation() {return m_bVisualisation;}
    static float getMaxScreenFracW() {return m_fMaxScreenFracW;}
    static float getMaxScreenFracH() {return m_fMaxScreenFracH;}
    static bool  getVisPollinatorTrails() {return m_bVisPollinatorTrails;}
    static int   getVisUpdatePeriod() {return m_iVisUpdatePeriod;}
    static int   getVisDelayPerFrame() {return m_iVisDelayPerFrame;}
    static bool  logging() {return m_bLogging;}
    static bool  logPollinatorsFull() {return m_bLogPollinatorsFull;}
    static bool  logFlowersFull() {return m_bLogFlowersFull;}
    static bool  logFlowersSummary() {return m_bLogFlowersSummary;}
    static int   getLogUpdatePeriod() {return m_iLogUpdatePeriod;}
    static int   getSimTerminationNumGens() {return m_iSimTerminationNumGens;}
    static GenTerminationType getGenTerminationType() {return m_GenTerminationType;}
    static int   getGenTerminationIntParam() {return m_iGenTerminationParam;}
    static float getGenTerminationFloatParam() {return m_fGenTerminationParam;}
    static MarkerPoint getEnvBackgroundReflectanceMP() {return m_EnvBackgroundReflectanceMP;}
    static const std::string & getRngSeedStr() {return m_strRngSeed;}
    static const std::string & getLogDir() {return m_strLogDir;}
    static const std::string & getLogRunName() {return m_strLogRunName;}
    static const std::vector<HiveConfig> & getHiveConfigs() {return m_Hives;}
    static const std::vector<PlantTypeDistributionConfig> & getPlantTypeDistributionConfigs() {return m_PlantDists;}
    static const std::vector<PlantTypeConfig> & getPlantTypeConfigs() {return m_PlantTypes;}
    static const PlantTypeConfig* getPlantTypeConfig(std::string species);
    static PollinatorConfig* getPollinatorConfigPtr(const std::string& pollinatorName);   
    
    static nlohmann::json& getJson() {return m_Json;}

    static bool  initialised() {return m_bInitialised;}

private:
    static bool  m_bVisualisation;          ///< Use visualiation for this run?
    static int   m_iEnvSizeX;               ///< Environment size (num patches) in x direction
    static int   m_iEnvSizeY;               ///< Environment size (num patches) in y direction
    static float m_fMaxScreenFracW;         ///< Max fraction of screen size for vis window width
    static float m_fMaxScreenFracH;         ///< Max fraction of screen size for vis window height
    static float m_fEnvDefaultAmbientTemp;  ///< Default ambient temperature for all Patches (in Celsius)
    static MarkerPoint m_EnvBackgroundReflectanceMP; ///< Default background reflectance Marker Point
                                                     ///<   for each Patch
    static bool  m_bReproGlobalDensityContrained;    ///< During a reproduction cycle, is global
                                                     ///<   plant density constrained?
    static float m_fReproGlobalDensityMax;  ///< If m_bReproGlobalDensityContrained, what is
                                            ///<   the maximum allowed density?
    static bool  m_bVisPollinatorTrails;    ///< Display trails of pollinators' past movements?
    static int   m_iVisUpdatePeriod;        ///< Number of model steps between each update of 
                                            ///<   visualisation
    static int   m_iVisDelayPerFrame;       ///< Specifies a delay (in ms) per frame of the 
                                            ///<   visualisation code
    static bool  m_bLogging;                ///< Is logging required for this run?
    static bool  m_bLogPollinatorsFull;     ///< Log full pollinator info every m_iLogUpdatePeriod steps
    static bool  m_bLogFlowersFull;         ///< Log full flower info at end of each generation
    static bool  m_bLogFlowersSummary;      ///< Log summary flower info at end of each generation
    static int   m_iLogUpdatePeriod;        ///< Number of model steps between each update of logger
    static std::string m_strLogDir;         ///< Directory name for logging output
    static std::string m_strLogRunName;     ///< Run name to be used as prefix for log filenames    
    static bool  m_bInitialised;            ///< Flag to indicate that parmas have been intiialised
    static int   m_iSimTerminationNumGens;  ///< Terminate run after this number of generations
    static GenTerminationType m_GenTerminationType; ///< Method used to define termination 
                                                    ///<   criterion for a generation
    static int   m_iGenTerminationParam;    ///< Integer parameter associated with m_GenTerminationType
    static float m_fGenTerminationParam;    ///< Float parameter associated with m_GenTerminationType
    static std::string m_strRngSeed;        ///< Seed string used to seeed RNG
    static std::vector<HiveConfig> m_Hives; ///< Configuration info for each hive
    static std::vector<PlantTypeDistributionConfig> m_PlantDists; ///< Config of plant distributions
    static std::vector<PlantTypeConfig> m_PlantTypes; ///< Config of plant types
    static std::vector<PollinatorConfig> m_PollinatorConfigs; ///< Config info for pollinator types

    static nlohmann::json m_Json;           ///< JSON representation of all Model Params
};

#endif /* _MODELPARAMS_H */
