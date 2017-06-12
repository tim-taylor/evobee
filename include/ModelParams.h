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
    static void setVisualisation(bool vis);
    static void setMaxScreenFrac(float f);
    static void setMaxScreenFracW(float fw);
    static void setMaxScreenFracH(float fh);
    static void setVisPollinatorTrails(bool show);
    static void setVisUpdatePeriod(int p);
    static void setLogUpdatePeriod(int p);
    static void setInitialised();
    static void setTerminationNumSteps(int steps);
    static void setRngSeedStr(const std::string& seed);
    static void setLogDir(const std::string& dir);
    static void setLogRunName(const std::string& name);

    static void addHiveConfig(HiveConfig& hc);
    static void addPlantTypeDistributionConfig(PlantTypeDistributionConfig& pc);
    static void addPlantTypeConfig(PlantTypeConfig& pt);
    static void addPollinatorConfig(PollinatorConfig& pt);

    static int   getEnvSizeX() {return m_iEnvSizeX;}
    static int   getEnvSizeY() {return m_iEnvSizeY;}
    static float getEnvDefaultAmbientTemp() {return m_fEnvDefaultAmbientTemp;}
    static int   getNumPatches() {return m_iEnvSizeX * m_iEnvSizeY;}
    static bool  getVisualisation() {return m_bVisualisation;}
    static float getMaxScreenFracW() {return m_fMaxScreenFracW;}
    static float getMaxScreenFracH() {return m_fMaxScreenFracH;}
    static bool  getVisPollinatorTrails() {return m_bVisPollinatorTrails;}
    static int   getVisUpdatePeriod() {return m_iVisUpdatePeriod;}
    static int   getLogUpdatePeriod() {return m_iLogUpdatePeriod;}
    static int   getTerminationNumSteps() {return m_iTerminationNumSteps;}
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
    static MarkerPoint m_EnvBackgroundReflectanceMP; ///< Default background reflectance Marker Point for each Patch
    static bool  m_bVisPollinatorTrails;    ///< Display trails of pollinators' past movements?
    static int   m_iVisUpdatePeriod;        ///< Number of model steps between each update of visualisation
    static int   m_iLogUpdatePeriod;        ///< Number of model steps between each update of logger
    static bool  m_bInitialised;            ///< Flag to indicate that parmas have been intiialised
    static int   m_iTerminationNumSteps;    ///< Terminate run after this number of steps
    static std::string m_strRngSeed;        ///< Seed string used to seeed RNG
    static std::string m_strLogDir;         ///< Directory name for logging output
    static std::string m_strLogRunName;     ///< Run name to be used as prefix for log filenames
    static std::vector<HiveConfig> m_Hives; ///< Configuration info for each hive
    static std::vector<PlantTypeDistributionConfig> m_PlantDists; ///< Config of plant distributions
    static std::vector<PlantTypeConfig> m_PlantTypes; ///< Config of plant types
    static std::vector<PollinatorConfig> m_PollinatorConfigs; ///< Config info for pollinator types

    static nlohmann::json m_Json;           ///< JSON representation of all Model Params
};

#endif /* _MODELPARAMS_H */
