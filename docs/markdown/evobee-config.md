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

The following table describes each parameter that can be specified in the configuration file. Parameters are read in from the configuration file and parsed by code in source file `evobee.cpp`. They are then stored in a static instance of the `ModelParams` class. See comments in the files `evobee.cpp` and `ModelParams.h` for further details of each parameter and the allowed values for each one.

|Parameter name in JSON config file|Corresponding variable name in ModeParams.h|Type of variable|Description|
 |---|---|---|---|  
|visualisation|m_bVisualisation|bool|Use visualiation for this run?|  
|env-size-x|m_iEnvSizeX|int|Environment size (num patches) in x direction|  
|env-size-y|m_iEnvSizeY|int|Environment size (num patches) in y direction|  
|vis-max-screen-frac-w|m_fMaxScreenFracW|float|Max fraction of screen size for vis window width|  
|vis-max-screen-frac-h|m_fMaxScreenFracH|float|Max fraction of screen size for vis window height|  
|default-ambient-temp|m_fEnvDefaultAmbientTemp|float|Default ambient temperature for all Patches (in Celsius)|  
|background-reflectance-mp|m_EnvBackgroundReflectanceMP|MarkerPoint|Default background reflectance Marker Point for each Patch|  
|repro-global-density-constrained|m_bReproGlobalDensityContrained|bool|During a reproduction cycle, is global plant density constrained?|  
|repro-global-density-max|m_fReproGlobalDensityMax|float|If m_bReproGlobalDensityContrained, what is the maximum allowed density?|  
|vis-pollinator-trails|m_bVisPollinatorTrails|bool|Display trails of pollinators' past movements?|  
|vis_update_period|m_iVisUpdatePeriod|int|Number of model steps between each update of visualisation|  
|vis-delay-per-frame|m_iVisDelayPerFrame|int|Specifies a delay (in ms) per frame of the visualisation code|  
| |m_bLogging|bool|Is logging required for this run?|  
| |m_bLogPollinatorsIntraPhaseFull|bool|Log full pollinator info every m_iLogUpdatePeriod steps|  
| |m_bLogPollinatorsInterPhaseSummary|bool|Log summary pollinator info at end of each generation|  
| |m_bLogFlowersInterPhaseFull|bool|Log full flower info at end of each generation|  
| |m_bLogFlowersInterPhaseSummary|bool|Log summary flower info at end of each generation|  
| |m_bLogFlowersIntraPhaseSummary|bool|Log summary flower info every m_iLogUpdatePeriod steps|  
| |m_iLogUpdatePeriod|int|Number of model steps between each update of logger|  
|log-dir|m_strLogDir|std::string|Directory name for logging output during a run|  
|log-final-dir|m_strLogFinalDir|std::string|Directory to which to move all log files at end of run (if blank, files are kept in m_strLogDir)|  
|log-run-name|m_strLogRunName|std::string|Run name to be used as prefix for log filenames|  
|use-log-threads|m_bUseLogThreads|bool|Use a separate thread for writing log files?|  
|verbose|m_bVerbose|bool|Should progress messages be printed on stdout?|  
| |m_bCommandLineQuiet|bool|Was the -q option used on command line?|  
| |m_bInitialised|bool|Flag to indicate that parmas have been intiialised|  
| |m_iSimTerminationNumGens|int|Terminate run after this number of generations|  
| |m_GenTerminationType|GenTerminationType|Method used to define termination criterion for a generation|  
| |m_iGenTerminationParam|int|Integer parameter associated with m_GenTerminationType|  
| |m_fGenTerminationParam|float|Float parameter associated with m_GenTerminationType|  
|auto-distribs|m_bPtdAutoDistribs|bool|Use auto-generation tool for Plant Type Distributions?|  
|auto-distrib-num-rows |m_iPtdAutoDistribNumRows|int|PTD auto-generation number of rows of areas to generate|  
|auto-distrib-num-cols|m_iPtdAutoDistribNumCols|int|PTD auto-generation number of columns of areas to generate|  
|auto-distrib-density|m_fPtdAutoDistribDensity|float|PTD auto-generation density of plants in each area|  
|auto-distrib-area-margin|m_fPtdAutoDistribAreaMargin|float|PTD auto-generation margin without flowers in each area (expressed in percentage of area's smaller side length)|  
|auto-distrib-regular|m_bPtdAutoDistribRegular|bool|Are the patches distributed in a regular or stockastic pattern?|
| |m_strRngSeed|std::string|Seed string used to seeed RNG|  
| |m_Hives|std::vector\<HiveConfig\>|Configuration info for each hive|  
| |m_PlantDists|std::vector\<PlantTypeDistributionConfig\>|Config of plant distributions|  
| |m_PlantTypes|std::vector\<PlantTypeConfig\>|Config of plant types|  
| |m_PollinatorConfigs|std::vector\<PollinatorConfig\>|Config info for pollinator types|


<!--stackedit_data:
eyJoaXN0b3J5IjpbODc2OTIwNzc5LC0xODUxOTA0MDk0LC0yMD
k2OTY3Mzc3LC0xNzQ1NDQ3MDQ0LDQ2MTcyODUyMiwtMTE2Mjgx
OTU4OCwyMDU2NDUwMzY1XX0=
-->