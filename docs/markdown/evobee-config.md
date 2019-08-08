# EvoBee simulation configuration and execution

[[ [Return to top-level documentation menu](../../README.md) ]]

## Basic usage

When the program has been successfully [compiled](evobee-install.md), the executable file named `evobee` can be found in the `build` directory.

The evobee executable accepts a small number of command-line arguments. To query what these are, run the command

    ./evobee --help

This output should look like this:

>  Allowed options:      
>  
> Generic options:  
> -v [ --version ] display program version number  
> -h [ --help ] display this help message  
> -c [ --config ] arg (=evobee.cfg.json) configuration file  
> -q [ --quiet ] disable verbose progress messages on stdout

The vast majority of configuration options for the program are set using a configuration file rather than the command line. As shown in the output above, the default filename that `evobee` searches for is `evobee.cfg.json`, and it only searches in the current working directory. To specify a different name and location, use the -c flag when calling the program. For example:

    ./evobee -c /home/me/my-config-file.cfg.json

The format of the configuration file and the options that can be specified within it are explained in the following sections.

## Configuration file format

## Configuration options



|--|--|--|--|
## jdhsk
 
| |bool|m_bVisualisation|Use visualiation for this run?|  
|--|--|--|--|  
| |int|m_iEnvSizeX|Environment size (num patches) in x direction|  
|--|--|--|--|  
| |int|m_iEnvSizeY|Environment size (num patches) in y direction|  
|--|--|--|--|  
| |float|m_fMaxScreenFracW|Max fraction of screen size for vis window width|  
|--|--|--|--|  
| |float|m_fMaxScreenFracH|Max fraction of screen size for vis window height|  
|--|--|--|--|  
| |float|m_fEnvDefaultAmbientTemp|Default ambient temperature for all Patches (in Celsius)|  
|--|--|--|--|  
| |MarkerPoint|m_EnvBackgroundReflectanceMP|Default background reflectance Marker Point for each Patch|  
|--|--|--|--|  
| |bool|m_bReproGlobalDensityContrained|During a reproduction cycle, is global plant density constrained?|  
|--|--|--|--|  
| |float|m_fReproGlobalDensityMax|If m_bReproGlobalDensityContrained, what is the maximum allowed density?|  
|--|--|--|--|  
| |bool|m_bVisPollinatorTrails|Display trails of pollinators' past movements?|  
|--|--|--|--|  
| |int|m_iVisUpdatePeriod|Number of model steps between each update of visualisation|  
|--|--|--|--|  
| |int|m_iVisDelayPerFrame|Specifies a delay (in ms) per frame of the visualisation code|  
|--|--|--|--|  
| |bool|m_bLogging|Is logging required for this run?|  
|--|--|--|--|  
| |bool|m_bLogPollinatorsIntraPhaseFull|Log full pollinator info every m_iLogUpdatePeriod steps|  
|--|--|--|--|  
| |bool|m_bLogPollinatorsInterPhaseSummary|Log summary pollinator info at end of each generation|  
|--|--|--|--|  
| |bool|m_bLogFlowersInterPhaseFull|Log full flower info at end of each generation|  
|--|--|--|--|  
| |bool|m_bLogFlowersInterPhaseSummary|Log summary flower info at end of each generation|  
|--|--|--|--|  
| |bool|m_bLogFlowersIntraPhaseSummary|Log summary flower info every m_iLogUpdatePeriod steps|  
|--|--|--|--|  
| |int|m_iLogUpdatePeriod|Number of model steps between each update of logger|  
|--|--|--|--|  
| |std::string|m_strLogDir|Directory name for logging output during a run|  
|--|--|--|--|  
| |std::string|m_strLogFinalDir|Directory to which to move all log files at end of run (if blank, files are kept in m_strLogDir)|  
|--|--|--|--|  
| |std::string|m_strLogRunName|Run name to be used as prefix for log filenames|  
|--|--|--|--|  
| |bool|m_bUseLogThreads|Use a separate thread for writing log files?|  
|--|--|--|--|  
| |bool|m_bVerbose|Should progress messages be printed on stdout?|  
|--|--|--|--|  
| |bool|m_bCommandLineQuiet|Was the -q option used on command line?|  
|--|--|--|--|  
| |bool|m_bInitialised|Flag to indicate that parmas have been intiialised|  
|--|--|--|--|  
| |int|m_iSimTerminationNumGens|Terminate run after this number of generations|  
|--|--|--|--|  
| |GenTerminationType|m_GenTerminationType|Method used to define termination criterion for a generation|  
|--|--|--|--|  
| |int|m_iGenTerminationParam|Integer parameter associated with m_GenTerminationType|  
|--|--|--|--|  
| |float|m_fGenTerminationParam|Float parameter associated with m_GenTerminationType|  
|--|--|--|--|  
| |bool|m_bPtdAutoDistribs|Use auto-generation tool for Plant Type Distributions?|  
|--|--|--|--|  
| |int|m_iPtdAutoDistribNumRows|PTD auto-generation number of rows of areas to generate|  
|--|--|--|--|  
| |int|m_iPtdAutoDistribNumCols|PTD auto-generation number of columns of areas to generate|  
|--|--|--|--|  
| |float|m_fPtdAutoDistribDensity|PTD auto-generation density of plants in each area|  
|--|--|--|--|  
| |float|m_fPtdAutoDistribAreaMargin|PTD auto-generation margin without flowers in each area (expressed in percentage of area's smaller side length)|  
|--|--|--|--|  
| |bool|m_bPtdAutoDistribRegular|Are the patches distributed in a regular or stockastic pattern?|  
|--|--|--|--|  
| |unsigned|int|m_sNextFreePtdcId Each PlantTypeDistributionConfig gets its own unique id|  
|--|--|--|--|  
| |std::string|m_strRngSeed|Seed string used to seeed RNG|  
|--|--|--|--|  
| |std::vector<HiveConfig>|m_Hives|Configuration info for each hive|  
|--|--|--|--|  
| |std::vector<PlantTypeDistributionConfig>|m_PlantDists|Config of plant distributions|  
|--|--|--|--|  
| |std::vector<PlantTypeConfig>|m_PlantTypes|Config of plant types|  
|--|--|--|--|  
| |std::vector<PollinatorConfig>|m_PollinatorConfigs|Config info for pollinator types|  
|--|--|--|--|



|  |  |
|--|--|
|  |  |


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
eyJoaXN0b3J5IjpbLTE0MDU5NjI0ODksLTE3NDU0NDcwNDQsND
YxNzI4NTIyLC0xMTYyODE5NTg4LDIwNTY0NTAzNjVdfQ==
-->