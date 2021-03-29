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
    static void setLogInterGenUpdatePeriod(int p);
    static void setLogThreads(bool useThreads) {m_bUseLogThreads = useThreads;}
    static void setLogDir(const std::string& dir);
    static void setLogFinalDir(const std::string& dir);
    static void setLogRunName(const std::string& name);
    static void setVerbose(bool verbose) {m_bVerbose = verbose;}
    static void setCommandLineQuiet(bool quiet) {m_bCommandLineQuiet = quiet;}
    static void setInitialised();
    static void setSimTerminationNumGens(int gens);
    static void setGenTerminationType(const std::string& typestr);
    static void setGenTerminationParam(int p);
    static void setGenTerminationParam(float p);
    static void setGenTerminationIntParam(int p);
    static void setGenTerminationFloatParam(float p);
    static void setRngSeedStr(const std::string& seed, bool bRewriteJsonEntry = false);
    static void setPtdAutoDistribs(bool bAutoDistribs);
    static void setPtdAutoDistribNumRows(int rows);
    static void setPtdAutoDistribNumCols(int cols);
    static void setPtdAutoDistribEqualNums(bool equalnums);
    static void setPtdAutoDistribDensity(float density);
    static void setPtdAutoDistribAreaMargin(float margin);
    static void setPtdAutoDistribRegular(bool regular);
    static void setPtdAutoDistribSeedOutflowAllowed(bool allowed);

    static void addHiveConfig(HiveConfig& hc);
    static void addPlantTypeDistributionConfig(PlantTypeDistributionConfig& pc);
    static void addPlantTypeConfig(PlantTypeConfig& pt);
    static void addPollinatorConfig(PollinatorConfig& pt);
    static void setColourSystem(const std::string& cs);

    static void setTestNumber(unsigned int num);

    /// perform any necessary global post-processing after config file has been read in
    static void postprocess();

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
    static bool  logPollinatorsIntraPhaseFull() {return m_bLogPollinatorsIntraPhaseFull;}
    static bool  logPollinatorsInterPhaseFull() {return m_bLogPollinatorsInterPhaseFull;}
    static bool  logPollinatorsInterPhaseSummary() {return m_bLogPollinatorsInterPhaseSummary;}
    static bool  logFlowersInterPhaseFull() {return m_bLogFlowersInterPhaseFull;}
    static bool  logFlowersInterPhaseSummary() {return m_bLogFlowersInterPhaseSummary;}
    static bool  logFlowersIntraPhaseFull() {return m_bLogFlowersIntraPhaseFull;}
    static bool  logFlowersIntraPhaseSummary() {return m_bLogFlowersIntraPhaseSummary;}
    static bool  logFlowerMPsInterPhaseSummary() {return m_bLogFlowerMPsInterPhaseSummary;}
    static bool  logFlowerInfoInterPhaseSummary() {return m_bLogFlowerInfoInterPhaseSummary;}
    static bool  logFinalDirSet() {return !m_strLogFinalDir.empty();}
    static int   getLogUpdatePeriod() {return m_iLogUpdatePeriod;}
    static int   getLogInterGenUpdatePeriod() {return m_iLogInterGenUpdatePeriod;}
    static bool  useLogThreads() {return m_bUseLogThreads;}
    static bool  verbose() {return m_bVerbose;}
    static bool  commandLineQuiet() {return m_bCommandLineQuiet;}
    static int   getSimTerminationNumGens() {return m_iSimTerminationNumGens;}
    static GenTerminationType getGenTerminationType() {return m_GenTerminationType;}
    static int   getGenTerminationIntParam() {return m_iGenTerminationParam;}
    static float getGenTerminationFloatParam() {return m_fGenTerminationParam;}
    static MarkerPoint getEnvBackgroundReflectanceMP() {return m_EnvBackgroundReflectanceMP;}
    static const std::string& getRngSeedStr() {return m_strRngSeed;}
    static const std::string& getLogDir() {return m_strLogDir;}
    static const std::string& getLogFinalDir() {return m_strLogFinalDir;}
    static const std::string& getLogRunName() {return m_strLogRunName;}
    static const std::vector<HiveConfig>& getHiveConfigs() {return m_Hives;}
    static const std::vector<PlantTypeDistributionConfig>& getPlantTypeDistributionConfigs() {return m_PlantDists;}
    static const std::vector<PlantTypeConfig>& getPlantTypeConfigs() {return m_PlantTypes;}
    static const PlantTypeConfig* getPlantTypeConfig(std::string speciesName);
    static const PlantTypeConfig* getPlantTypeConfig(unsigned int speciesNum);
    static int   getNumPlantTypes() {return m_PlantTypes.size();}
    static PollinatorConfig* getPollinatorConfigPtr(const std::string& pollinatorName);
    static unsigned int getTestNumber() {return m_iTestNumber;}
    static ColourSystem getColourSystem() {return m_ColourSystem;}

    static nlohmann::json& getJson() {return m_Json;}

    static bool  initialised() {return m_bInitialised;}

private:

    // private helper methods
    static void autoGeneratePtds();
    static void initialiseAutoGenPtdSpeciesPatchMap(std::vector<const std::string*>& speciesPatchMap);
    static const std::string getAutoGenPtdSpeciesForPatch(int x, int y, std::vector<const std::string*>& speciesPatchMap);
    static void pairPlantTypeConfigsToVisData();
    static const std::vector<VisualStimulusInfo>& getVisData();


    // data members
    static bool  m_bVisualisation;          ///< Use visualisation for this run?
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
    static bool  m_bLogPollinatorsIntraPhaseFull;     ///< Log full pollinator info every m_iLogUpdatePeriod steps
    static bool  m_bLogPollinatorsInterPhaseFull;     ///< Log full pollinator info at end of each generation, every m_iLogInterGenUpdatePeriod gens
    static bool  m_bLogPollinatorsInterPhaseSummary;  ///< Log summary pollinator info at end of each generation, every m_iLogInterGenUpdatePeriod gens
    static bool  m_bLogFlowersInterPhaseFull;         ///< Log full flower info at end of each generation, every m_iLogInterGenUpdatePeriod gens
    static bool  m_bLogFlowersInterPhaseSummary;      ///< Log summary flower info at end of each generation, every m_iLogInterGenUpdatePeriod gens
    static bool  m_bLogFlowersIntraPhaseFull;         ///< Log full flower info every m_iLogUpdatePeriod steps
    static bool  m_bLogFlowersIntraPhaseSummary;      ///< Log summary flower info every m_iLogUpdatePeriod steps
    static bool  m_bLogFlowerMPsInterPhaseSummary;    ///< Log summary of flower marker points at end of each generation, every m_iLogInterGenUpdatePeriod gens
    static bool  m_bLogFlowerInfoInterPhaseSummary;   ///< Log summary of flower info aggregared by VisualStimulusInfo id at each of each gen, every m_iLogInterGenUpdatePeriod gens
    static int   m_iLogUpdatePeriod;        ///< Number of model steps between each update of logger for log..IntraPhase methods
    static int   m_iLogInterGenUpdatePeriod;///< Number of generations between each update of logger for log..InterPhase methods
    static std::string m_strLogDir;         ///< Directory name for logging output during a run
    static std::string m_strLogFinalDir;    ///< Directory to which to move all log files at end of run
                                            ///<   (if blank, files are kept in m_strLogDir)
    static std::string m_strLogRunName;     ///< Run name to be used as prefix for log filenames
    static bool  m_bUseLogThreads;          ///< Use a separate thread for writing log files?
    static bool  m_bVerbose;                ///< Should progress messages be printed on stdout?
    static bool  m_bCommandLineQuiet;       ///< Was the -q option used on command line?
    static bool  m_bInitialised;            ///< Flag to indicate that parmas have been intiialised
    static int   m_iSimTerminationNumGens;  ///< Terminate run after this number of generations
    static GenTerminationType m_GenTerminationType; ///< Method used to define termination
                                                    ///<   criterion for a generation
    static int   m_iGenTerminationParam;    ///< Integer parameter associated with m_GenTerminationType
    static float m_fGenTerminationParam;    ///< Float parameter associated with m_GenTerminationType
    static bool  m_bPtdAutoDistribs;        ///< Use auto-generation tool for Plant Type Distributions?
    static int   m_iPtdAutoDistribNumRows;  ///< PTD auto-generation number of rows of areas to generate
    static int   m_iPtdAutoDistribNumCols;  ///< PTD auto-generation number of columns of areas to generate
    static bool  m_bPtdAutoDistribEqualNums;///< PTD auto-generation create an equal number of patches for each plant type
                                            ///<   (leaving any remaining patches empty)?
    static float m_fPtdAutoDistribDensity;  ///< PTD auto-generation density of plants in each area
    static float m_fPtdAutoDistribAreaMargin;   ///< PTD auto-generation margin without flowers in each area
                                                ///< (expressed in percentage of area's smaller side length)
    static bool  m_bPtdAutoDistribRegular;  ///< Are the patches distributed in a regular or stochastic pattern?
    static bool  m_bPtdAutoDistribSeedOutflowAllowed; ///< Can seeds spread beyond patches between generations?
    static std::string  m_strNoSpecies;     ///< String representing the absence of a plant species in an area, used internally only
    static unsigned int m_sNextFreePtdcId;  ///< Each PlantTypeDistributionConfig gets its own unique id
    static std::string m_strRngSeed;        ///< Seed string used to seed RNG
    static std::vector<HiveConfig> m_Hives; ///< Configuration info for each hive
    static std::vector<PlantTypeDistributionConfig> m_PlantDists;   ///< Config of plant distributions
    static std::vector<PlantTypeConfig> m_PlantTypes;               ///< Config of plant types
    static std::vector<PollinatorConfig> m_PollinatorConfigs;       ///< Config info for pollinator types
    static ColourSystem m_ColourSystem;     ///< Defines how we interpret colour stimuli

    static unsigned int m_iTestNumber;      ///< Specifies that we should run a special test on the
                                            ///<   code rather than a normal run (default value is 0
                                            ///<   which means do a normal run).

    static bool m_bSyntheticRegularMarkerPointsAdded; ///< This flag is used for internal checking during system initialisation

    static nlohmann::json m_Json;           ///< JSON representation of all Model Params
};

#endif /* _MODELPARAMS_H */
