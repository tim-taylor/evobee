# EvoBee simulation configuration and execution

## Basic usage

When the program has been successfully [compiled](evobee-install.md), the executable file named `evobee` can be found in the `build` directory.

The evobee executable accepts a small number of command-line arguments. To query what these are, run the command

    > ./evobee --help

This output should look like this:

>  Allowed options:      
>  
> Generic options:  
> -v [ --version ] display program version number  
> -h [ --help ] display this help message  
> -c [ --config ] arg (=evobee.cfg.json) configuration file  
> -q [ --quiet ] disable verbose progress messages on stdout

The vast majority of configuration options for the program are set using a configuration file rather than the command line. As shown in the output above, the default filename that `evobee` searches for is `evobee.cfg.json`, and it only searches in the current working directory. To specify a different name and location, use the -c flag when calling the program. For example:

    > ./evobee -c /home/me/my-config-file.cfg.json

The format of the configuration file and the options that can be specified within it are explained in the following sections.

## Configuration file format

EvoBee configuration files are specified in JSON format. Two sample configuration files are provided in the root directory of the source code. These are named `evobee.cfg.json` and `evobee-test-simple.cfg.json`. These two files demonstrate different aspects of system configuration, and it is recommended to study both of them.

These files are shown below, followed by the [Configuration options](#configuration-options) section which gives further details of each available configuration option.

### evobee.cfg.json sample configuration file

    {
        "SimulationParams" : {
            "generation-termination-type" : "pollinated-fraction-all",
            "generation-termination-param" : 0.5,
            "sim-termination-num-gens" : 1,
            "rng-seed" : "",
            "visualisation" : true,
            "vis-update-period" : 1,
            "vis-delay-per-frame" : 0,
            "vis-pollinator-trails" : true,
            "vis-max-screen-frac-w" : 0.85,
            "vis-max-screen-frac-h" : 0.85,
            "logging" : true,
            "log-flags" : "f",
            "log-update-period" : 5,
            "log-dir" : "output",
            "log-final-dir" : "",
            "log-run-name" : "auto-distribs-test",
            "verbose" : false
        },
        "Environment" : {
            "env-size-x" : 200,
            "env-size-y" : 200,
            "background-reflectance-mp" : 500,
            "default-ambient-temp": 20.0,
            "repro-global-density-constrained" : true,
            "repro-global-density-max" : 0.4,
            "Hives" : {
                "Hive1" : {
                    "pollinator-type" : "HoneyBee",
                    "pollinator-number" : 400,
                    "start-from-hive" : false,
                    "pos-x" : 99,
                    "pos-y" : 99,
                    "area-top-left-x" : 0,
                    "area-top-left-y" : 0,
                    "area-bottom-right-x" : 199,
                    "area-bottom-right-y" : 199,
                    "migration-allowed" : true,
                    "migration-restricted" : false,
                    "migration-prob" : 0.5
                }
            },
            "PlantTypeDistributions" : {
                "auto-distribs" : true,
                "auto-distrib-num-rows" : 10,
                "auto-distrib-num-cols" : 10,
                "auto-distrib-area-margin" : 0.0,
                "auto-distrib-density" : 0.5,
    	    "auto-distrib-regular" : true
            }
        },
        "PlantTypes" : {
            "PlantType1" : {
                "species" : "PlantSpecies1",
                "flower-reflectance-mp-init-min" : 380,
                "flower-reflectance-mp-init-max" : 380,
                "anther-init-pollen" : 100,
                "anther-pollen-transfer-per-visit" : 10,
                "stigma-max-pollen-capacity" : 5,
                "pollen-clogging" : "",
                "repro-seed-dispersal-global" : true,
                "repro-seed-dispersal-radius" : 1.0
            },
            "PlantType2" : {
                "species" : "PlantSpecies2",
                "flower-reflectance-mp-init-min" : 480,
                "flower-reflectance-mp-init-max" : 480,
                "anther-init-pollen" : 100,
                "anther-pollen-transfer-per-visit" : 10,
                "stigma-max-pollen-capacity" : 5,
                "pollen-clogging" : "",
                "repro-seed-dispersal-global" : true,
                "repro-seed-dispersal-radius" : 1.0
            }
        },
        "Pollinators" : {
            "Pollinator1" : {
                "species" : "HoneyBee",
                "bout-length" : 100,
                "max-pollen-capacity" : 0,
                "pollen-deposit-per-flower-visit" : 3,
                "pollen-loss-in-air" : 0,
                "pollen-carryover-num-visits" : 100,
                "constancy-type" : "none",
                "foraging-strategy" : "nearest-flower",
                "visited-flower-memory-size" : 5
            }
        }
    }

### evobee-test-simple.cfg.json sample configuration file

    {
        "SimulationParams" : {
            "generation-termination-type" : "num-sim-steps",
            "comment1" : "[num-sim-steps|pollinated-fraction|<num-pollinator-steps>]",
            "generation-termination-param" : 50,
            "sim-termination-num-gens" : 50,
            "rng-seed" : "",
            "visualisation" : false,
            "vis-update-period" : 100,
            "vis-delay-per-frame" : 0,
            "vis-pollinator-trails" : true,
            "vis-max-screen-frac-w" : 0.85,
            "vis-max-screen-frac-h" : 0.85,
            "logging" : true,
            "log-flags" : "f",
            "comment-log-flags-1" : "P: all pollinator positions every log-update-period steps",
            "comment-log-fiags-2" : "p: summary pollinator stats at end of each foraging phase",
            "comment-log-flags-3" : "F: all flowers at end of each foraging phase",
            "comment-log-flags-4" : "f: summary flower stats at end of each foraging phase",
            "log-update-period" : 5,
            "log-dir" : "output/test",
            "log-run-name" : "simple",
            "verbose" : true
        },
        "Environment" : {
            "env-size-x" : 100,
            "env-size-y" : 100,
            "background-reflectance-mp" : 500,
            "default-ambient-temp": 20.0,
            "repro-global-density-constrained" : true,
            "repro-global-density-max" : 0.2,
            "Hives" : {
                "Hive1" : {
                    "pollinator-type" : "HoneyBee",
                    "pollinator-number" : 100,
                    "start-from-hive" : false,
                    "pos-x" : 49,
                    "pos-y" : 49,
                    "area-top-left-x" : 0,
                    "area-top-left-y" : 0,
                    "area-bottom-right-x" : 99,
                    "area-bottom-right-y" : 99,
                    "migration-allowed" : true,
                    "migration-restricted" : false,
                    "migration-prob" : 0.5
                }
            },
            "PlantTypeDistributions" : {
                "PlantTypeDistribution1" : {
                    "species" : "PlantSpecies1",
                    "area-top-left-x" : 0,
                    "area-top-left-y" : 0,
                    "area-bottom-right-x" : 99,
                    "area-bottom-right-y" : 99,
                    "density" : 0.5,
                    "refuge" : false,
                    "seed-outflow-allowed" : true,
                    "seed-outflow-restricted" : false,
                    "seed-outflow-prob" : 0.5,
                    "repro-local-density-constrained" : false,
                    "repro-local-density-max" : 0.2
                },
                "PlantTypeDistribution2" : {
                    "species" : "PlantSpecies2",
                    "area-top-left-x" : 0,
                    "area-top-left-y" : 0,
                    "area-bottom-right-x" : 99,
                    "area-bottom-right-y" : 99,
                    "density" : 0.5,
                    "refuge" : false,
                    "seed-outflow-allowed" : true,
                    "seed-outflow-restricted" : false,
                    "seed-outflow-prob" : 0.5,
                    "repro-local-density-constrained" : false,
                    "repro-local-density-max" : 0.2
                }
            }
        },
        "PlantTypes" : {
            "PlantType1" : {
                "species" : "PlantSpecies1",
                "flower-reflectance-mp-init-min" : 360,
                "flower-reflectance-mp-init-max" : 430,
                "anther-init-pollen" : 100,
                "anther-pollen-transfer-per-visit" : 10,
                "stigma-max-pollen-capacity" : 5,
                "pollen-clogging" : "",
                "repro-seed-dispersal-global" : true,
                "repro-seed-dispersal-radius" : 1.0,
                "comment0" : "items below this line in PlantType not currently used",
                "nectar-reward" : 10,
                "comment1" : "plus replenish rate for all of these things - ",
                "comment2" : "if second bee visits, does it get the same reward or none;",
                "comment3" : "replenish, instant or n per timestep",
                "comment4" : "flower handling time; self-pollination allowed"
            },
            "PlantType2" : {
                "species" : "PlantSpecies2",
                "flower-reflectance-mp-init-min" : 431,
                "flower-reflectance-mp-init-max" : 500,
                "anther-init-pollen" : 100,
                "anther-pollen-transfer-per-visit" : 10,
                "stigma-max-pollen-capacity" : 5,
                "pollen-clogging" : "",
                "repro-seed-dispersal-global" : true,
                "repro-seed-dispersal-radius" : 1.0
            }
        },
        "Pollinators" : {
            "Pollinator1" : {
                "species" : "HoneyBee",
                "bout-length" : 100,
                "max-pollen-capacity" : 0,
                "pollen-deposit-per-flower-visit" : 3,
                "pollen-loss-in-air" : 0,
                "pollen-carryover-num-visits" : 100,
                "innate-mp-pref-min" : 400,
                "innate-mp-pref-max" : 500,
                "constancy-type" : "simple",
                "constancy-param" : 1.0,
                "foraging-strategy" : "nearest-flower",
                "visited-flower-memory-size" : 20
            }
        }
    }

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
|auto-distrib-regular|m_bPtdAutoDistribRegular|bool|Are the patches distributed in a regular or stochastic pattern?|
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
|species|species|std::string|Text label designating species of pollinator (any text is allowed)|  
|bout-length|boutLength|int|Number of flower visits allowed before returning to hive [0=unlimited]|  
|max-pollen-capacity|maxPollenCapacity|int|Maximum amount of pollen the pollinator can carry|  
|pollen-deposit-per-flower-visit|pollenDepositPerFlowerVisit|int|Amount of pollen deposited on a flower on each visit|  
|pollen-loss-in-air|pollenLossInAir|int|Amount of pollen lost on each timestep when flying|  
|pollen-carryover-num-visits|pollenCarryoverNumVisits|int|After collecting a grain of pollen from a flower, the pollinator can visit this number of subsequent flowers to potentially deposit it. If the pollen is still not deposited after this number of visits, it is removed from the pollinator (i.e. it is lost). A value of 0 indicates no limit in number of visits.|  
|innate-mp-pref-min|innateMPPrefMin|MarkerPoint||  
|innate-mp-pref-max|innateMPPrefMax|MarkerPoint||  
|constancy-type|strConstancyType|std::string|Allowed values: none, simple|  
|constancy-param|constancyParam|float|If constancy-type=simple, constancy-param is a number between 0.0 and 1.0. In this case, when the pollinator sees a flower of the same species it has landed on before, it lands on it with probability 0.9. If it is of a different species, it lands on it with probability (1 - constancy-param).|  
|foraging-strategy|strForagingStrategy|std::string|Allowed values: random, nearest-flower, random-flower, random-global|  
|visited-flower-memory-size|visitedFlowerMemorySize|unsigned int|The maximum number of recently visited flowers that the pollinator can remember. This may affect whether or not it relands on a given flower, depending upon its foraging strategy.|


### Plant Type configuration parameters

Plant Type configuration parameters for a single plant type are stored in an instance of the `PlantTypeConfig` structure, defined in source file `PlantTypeConfig.h`. These are stored in the `ModelParams` member `m_PlantTypes`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeConfig.h|Type of variable|Description|
|---|---|---|---|  
|species|species|std::string|Text label designating species of plant (any text is allowed)|  
|flower-reflectance-mp-init-min|flowerMPInitMin|MarkerPoint||  
|flower-reflectance-mp-init-max|flowerMPInitMax|MarkerPoint||  
|anther-init-pollen|antherInitPollen|int||  
|anther-pollen-transfer-per-visit|antherPollenTransferPerVisit|int||  
|stigma-max-pollen-capacity|stigmaMaxPollenCapacity|int||  
|pollen-clogging|pollenCloggingSpecies|std::string||  
|repro-seed-dispersal-global|reproSeedDispersalGlobal|bool|can seeds be dispersed at random across whole environment?|  
|repro-seed-dispersal-radius|reproSeedDispersalRadius|float|expreseed in env units (1.0=one patch), overridden if reproSeedDispersalGlobal=true|


### Plant Distribution configuration parameters

Plant distribution configuration parameters for a specified plant type in a specified region of the environment are stored in an instance of the `PlantTypeDistributionConfig` structure, defined in source file `PlantTypeDistributionConfig.h`. These are stored in the `ModelParams` member `m_PlantDists`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeDistributionConfig.h|Type of variable|Description|
|---|---|---|---|  
|species|species|std::string|Name of the plant species|  
|area-top-left-x, area-top-left-y|areaTopLeft|iPos (Position\<int\>)|Top-left corner of initial distribution area|  
|area-bottom-right-x, area-bottom-right-y|areaBottomRight|iPos (Position\<int\>)|Bottom-right corner of initial distribution area|  
|density|density|float|Density of initial distribution (wrt specified distrib area)|  
|refuge|refuge|bool|Is this area a refuge for this plant species?|  
|refuge-alien-inflow-prob|refugeAlienInflowProb|float|If this is a refuge, what is the probability that an alien species attempting to germinate in this area will do so successfully|  
|seed-outflow-allowed|seedOutflowAllowed|bool|Can the plants germinate outside of the initial distrib area?|  
|seed-outflow-restricted|seedOutflowRestricted|bool|If seedOutflowAllowed, is it restricted?|  
|seed-outflow-prob|seedOutflowProb|float|If seedOutflowAllowed and seedOutflowRestricted, what is the probability that they are successful if attempting to do so?|  
|repro-local-density-constrained|reproLocalDensityConstrained|bool|During reproduction, is the maximum density of plants in this local area constrained?|  
|repro-local-density-max|reproLocalDensityMax|float|If reproLocalDensityConstrained, this is the maximum allowed density|

<!--stackedit_data:
eyJoaXN0b3J5IjpbOTEyNzUxNTg2LDM3MTg4NzE2MywtNzA4ND
QyMTc4LDY2MTc0NDMxMSwxODI5MjY4NjcwLDczMTc1NjA4OSwx
MDY4Njg1MjM2LC0xNTAzNTQxMzUxLDY3NTcxMjAwMSwtMTA4NT
I1NjIxOSwxNjk2ODA5OTUsLTg4Mjg0OTY0LC0xMDg1MjU2MjE5
LDEzMDIyNTc0MDksMTMwNzQ3NzYyOSwtMTg1MTkwNDA5NCwtMj
A5Njk2NzM3NywtMTc0NTQ0NzA0NCw0NjE3Mjg1MjIsLTExNjI4
MTk1ODhdfQ==
-->