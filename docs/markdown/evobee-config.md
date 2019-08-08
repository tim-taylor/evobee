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

### General parameters

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
|logging|m_bLogging|bool|Is logging required for this run?|  
|log-flags|m_bLogPollinatorsIntraPhaseFull, m_bLogPollinatorsInterPhaseSummary, m_bLogFlowersInterPhaseFull, m_bLogFlowersInterPhaseSummary, m_bLogFlowersIntraPhaseSummary|std::string|Flags to control logging functionality (Q=PollinatorsIntraPhaseFull, p=PollinatorsInterPhaseSummary, F=FlowersInterPhaseFull, f=FlowersInterPhaseSummary, g=FlowersIntraPhaseSummary)|
|log-update-period|m_iLogUpdatePeriod|int|Number of model steps between each update of logger|  
|log-dir|m_strLogDir|std::string|Directory name for logging output during a run|  
|log-final-dir|m_strLogFinalDir|std::string|Directory to which to move all log files at end of run (if blank, files are kept in m_strLogDir)|  
|log-run-name|m_strLogRunName|std::string|Run name to be used as prefix for log filenames|  
|use-log-threads|m_bUseLogThreads|bool|Use a separate thread for writing log files?|  
|verbose|m_bVerbose|bool|Should progress messages be printed on stdout?|
|sim-termination-num-gens|m_iSimTerminationNumGens|int|Terminate run after this number of generations|  
|generation-termination-type|m_GenTerminationType|GenTerminationType|Method used to define termination criterion for a generation. Allowed values: num-sim-steps, num-pollinator-steps, pollinated-fraction, pollinated-fraction-all, pollinated-fraction-species1.|  
|generation-termination-param|m_iGenTerminationParam or m_fGenTerminationParam|int or float|Parameter associated with m_GenTerminationType (type of param depends on value of generation-termination-type)|  
|auto-distribs|m_bPtdAutoDistribs|bool|Use auto-generation tool for Plant Type Distributions?|  
|auto-distrib-num-rows |m_iPtdAutoDistribNumRows|int|PlantTypeDistribution auto-generation number of rows of areas to generate|  
|auto-distrib-num-cols|m_iPtdAutoDistribNumCols|int|PlantTypeDistribution auto-generation number of columns of areas to generate|  
|auto-distrib-density|m_fPtdAutoDistribDensity|float|PlantTypeDistribution auto-generation density of plants in each area|  
|auto-distrib-area-margin|m_fPtdAutoDistribAreaMargin|float|PlantTypeDistribution auto-generation margin without flowers in each area (expressed in percentage of area's smaller side length)|  
|auto-distrib-regular|m_bPtdAutoDistribRegular|bool|Are the patches distributed in a regular or stockastic pattern?|
|rng-seed|m_strRngSeed|std::string|Seed string used to seeed RNG|

### Hive configuration parameters

Hive configuration parameters for a single hive are stored in an instance of the `HiveConfig` structure, defined in source file `HiveConfig.h`. These are stored in the `ModelParams` member `m_Hives`.

|Parameter name in JSON config file|Corresponding variable name in HiveConfig.h|Type of variable|Description|
|---|---|---|---|  
|pollinator-type|type|std::string|Pollinator type|  
|pollinator-number|num|int|Number of pollinators|  
|start-from-hive|startFromHive|bool|Do all pollinators start at hive, or from random pos in initial foraging area?|  
|pos-x, pos-y|position|fPos (Position\<float\>)|Position of the hive|  
|area-top-left-x, area-top-left-y|areaTopLeft|iPos (Position\<int\>)|Top-left corner of initial foraging area|  
|area-bottom-right-x, area-bottom-right-y|areaBottomRight|iPos (Position\<int\>)|Bottom-right corner of initial foraging area|  
|migration-allowed|migrationAllowed|bool|Are pollinators allowed to move outside initial foraging area?|  
|migration-restricted|migrationRestricted|bool|If migration allowed, is its probability restricted at all?|  
|migration-prob|migrationProb|float|If migration allowed and restricted, and pollinator tries to leave initial foraging area, it succeeds with this prob, else it is reflected back into initial area|

### Pollinator configuration parameters

Pollinator configuration parameters for a single pollinator species are stored in an instance of the `PollinatorConfig` structure, defined in source file `PollinatorConfig.h`. These are stored in the `ModelParams` member `m_PollinatorConfigs`.

|Parameter name in JSON config file|Corresponding variable name in PollinatorConfig.h|Type of variable|Description|
|---|---|---|---|  




### Plant Type configuration parameters

Plant Type configuration parameters for a single plant type are stored in an instance of the `PlantTypeConfig` structure, defined in source file `PlantTypeConfig.h`. These are stored in the `ModelParams` member `m_PlantTypes`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeConfig.h|Type of variable|Description|
|---|---|---|---|  


### Plant Distribution configuration parameters

Plant distribution configuration parameters for a specified plant type in a specified region of the environment are stored in an instance of the `PlantTypeDistributionConfig` structure, defined in source file `PlantTypeConfig.h`. These are stored in the `ModelParams` member `m_PlantTypes`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeConfig.h|Type of variable|Description|
|---|---|---|---|  


| |m_PlantDists|std::vector\<PlantTypeDistributionConfig\>|Config of plant distributions|  
| |m_PlantTypes|std::vector\<PlantTypeConfig\>|Config of plant types|  
| |m_PollinatorConfigs|std::vector\<PollinatorConfig\>|Config info for pollinator types|


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE1MDU0Njg0NDYsNzMxNzU2MDg5LDEwNj
g2ODUyMzYsLTE1MDM1NDEzNTEsNjc1NzEyMDAxLC0xMDg1MjU2
MjE5LDE2OTY4MDk5NSwtODgyODQ5NjQsLTEwODUyNTYyMTksMT
MwMjI1NzQwOSwxMzA3NDc3NjI5LC0xODUxOTA0MDk0LC0yMDk2
OTY3Mzc3LC0xNzQ1NDQ3MDQ0LDQ2MTcyODUyMiwtMTE2MjgxOT
U4OCwyMDU2NDUwMzY1XX0=
-->