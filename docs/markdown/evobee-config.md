# EvoBee simulation configuration and execution

[[ [Return to top-level documentation menu](../../README.md) ]]

## In

## Parameters

// data members

static  bool m_bVisualisation; ///< Use visualiation for this run?

static  int m_iEnvSizeX; ///< Environment size (num patches) in x direction

static  int m_iEnvSizeY; ///< Environment size (num patches) in y direction

static  float m_fMaxScreenFracW; ///< Max fraction of screen size for vis window width

static  float m_fMaxScreenFracH; ///< Max fraction of screen size for vis window height

static  float m_fEnvDefaultAmbientTemp; ///< Default ambient temperature for all Patches (in Celsius)

static MarkerPoint m_EnvBackgroundReflectanceMP; ///< Default background reflectance Marker Point

///< for each Patch

static  bool m_bReproGlobalDensityContrained; ///< During a reproduction cycle, is global

///< plant density constrained?

static  float m_fReproGlobalDensityMax; ///< If m_bReproGlobalDensityContrained, what is

///< the maximum allowed density?

static  bool m_bVisPollinatorTrails; ///< Display trails of pollinators' past movements?

static  int m_iVisUpdatePeriod; ///< Number of model steps between each update of

///< visualisation

static  int m_iVisDelayPerFrame; ///< Specifies a delay (in ms) per frame of the

///< visualisation code

static  bool m_bLogging; ///< Is logging required for this run?

static  bool m_bLogPollinatorsIntraPhaseFull; ///< Log full pollinator info every m_iLogUpdatePeriod steps

static  bool m_bLogPollinatorsInterPhaseSummary; ///< Log summary pollinator info at end of each generation

static  bool m_bLogFlowersInterPhaseFull; ///< Log full flower info at end of each generation

static  bool m_bLogFlowersInterPhaseSummary; ///< Log summary flower info at end of each generation

static  bool m_bLogFlowersIntraPhaseSummary; ///< Log summary flower info every m_iLogUpdatePeriod steps

static  int m_iLogUpdatePeriod; ///< Number of model steps between each update of logger

static  std::string m_strLogDir; ///< Directory name for logging output during a run

static  std::string m_strLogFinalDir; ///< Directory to which to move all log files at end of run

///< (if blank, files are kept in m_strLogDir)

static  std::string m_strLogRunName; ///< Run name to be used as prefix for log filenames

static  bool m_bUseLogThreads; ///< Use a separate thread for writing log files?

static  bool m_bVerbose; ///< Should progress messages be printed on stdout?

static  bool m_bCommandLineQuiet; ///< Was the -q option used on command line?

static  bool m_bInitialised; ///< Flag to indicate that parmas have been intiialised

static  int m_iSimTerminationNumGens; ///< Terminate run after this number of generations

static GenTerminationType m_GenTerminationType; ///< Method used to define termination

///< criterion for a generation

static  int m_iGenTerminationParam; ///< Integer parameter associated with m_GenTerminationType

static  float m_fGenTerminationParam; ///< Float parameter associated with m_GenTerminationType

static  bool m_bPtdAutoDistribs; ///< Use auto-generation tool for Plant Type Distributions?

static  int m_iPtdAutoDistribNumRows; ///< PTD auto-generation number of rows of areas to generate

static  int m_iPtdAutoDistribNumCols; ///< PTD auto-generation number of columns of areas to generate

static  float m_fPtdAutoDistribDensity; ///< PTD auto-generation density of plants in each area

static  float m_fPtdAutoDistribAreaMargin; ///< PTD auto-generation margin without flowers in each area

///< (expressed in percentage of area's smaller side length)

static  bool m_bPtdAutoDistribRegular; ///< Are the patches distributed in a regular or stockastic pattern?

static  unsigned  int m_sNextFreePtdcId; ///< Each PlantTypeDistributionConfig gets its own unique id

static  std::string m_strRngSeed; ///< Seed string used to seeed RNG

static  std::vector<HiveConfig> m_Hives; ///< Configuration info for each hive

static  std::vector<PlantTypeDistributionConfig> m_PlantDists; ///< Config of plant distributions

static  std::vector<PlantTypeConfig> m_PlantTypes; ///< Config of plant types

static  std::vector<PollinatorConfig> m_PollinatorConfigs; ///< Config info for pollinator types
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE1OTQ1NTU1NTYsMjA1NjQ1MDM2NV19
-->