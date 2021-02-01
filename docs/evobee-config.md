# EvoBee simulation configuration and execution

## Basic usage

When the program has been successfully [compiled](evobee-install.md), the executable file named `evobee` can be found in the `build` directory.

The evobee executable accepts a small number of command-line arguments. To query what these are, run the command

    > ./evobee --help

This output should look like this:

> Generic options:
> 
> -v [ --version ] -> display program version number  
> -h [ --help ] -> display this help message  
> -c [ --config ] arg (=evobee.cfg.json) -> configuration file  
> -q [ --quiet ] -> disable verbose progress messages on stdout  
> -t [ --test ] arg (=0) -> Perform test number N instead of regular run  

*The final option, -t, is used to perform various tests on the code rather than a regular run. There are currently two tests defined: 1=MarkerPointSimilarityTest and 2=MatchConfidenceTest. For more information on these tests see the EvoBeeExperiment.cpp file, which calls the tests from the method EvoBeeExperiment::run().*

The vast majority of configuration options for the program are set using a configuration file rather than the command line. As shown in the output above, the default filename that `evobee` searches for is `evobee.cfg.json`, and it only searches in the current working directory. To specify a different name and location, use the -c flag when calling the program. For example:

    > ./evobee -c /home/me/my-config-file.cfg.json

The format of the configuration file and the options that can be specified within it are explained in the following sections.

## Configuration file format

EvoBee configuration files are specified in JSON format. These follow a hierarchical structure with the following outline:

    {
        "SimulationParams" : {
            # general params for stopping criteria, logging,
            # visualisation, etc
        },
        "Environment" : {
            # configuration of environment size, density
            # parameters, and other global specs, followed by:
            "Hives" : {
                # specification of one or more "Hive" section
                # detailing hive position, pollinator type,
                # initial foraging area, etc.
            },
            "PlantTypeDistributions" : {
                # specification of one or more "PlantTypeDistribution"
                # sections detailing species of plant, its initial
                # distribution area and related parameters. Note that
                # multiple PlantTypeDistributions can overlap in
                # their specified areas. Alternatively, rather than
                # explicity defining PlantTypeDistribution areas, this
                # section can specify "auto-distribs : true" and use
                # the related parameters to have EvoBee automatically
                # distribute the plants described in the "PlantTypes"
                # section across the environment in the requested way.
            }
        },
        "PlantTypes" : {
            # specification of one or more "PlantType" to be used
            # in the simulation. These types are referred to in the
            # "PlantTypeDistributions" section above
        },
        "Pollinators" : {
            # specification of one or more "Pollinator" to to used
            # in the simulation. These are referred to in the
            # "Hives" section above
        }
    }

Two sample configuration files are provided in the root directory of the source code. These are named `evobee.cfg.json` and `evobee-test-simple.cfg.json`. These two files demonstrate different aspects of system configuration, and it is recommended to study both of them.

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
                "repro-seed-dispersal-global" : true
            },
            "PlantType2" : {
                "species" : "PlantSpecies2",
                "flower-reflectance-mp-init-min" : 480,
                "flower-reflectance-mp-init-max" : 480,
                "anther-init-pollen" : 100,
                "anther-pollen-transfer-per-visit" : 10,
                "stigma-max-pollen-capacity" : 5,
                "pollen-clogging" : "",
                "repro-seed-dispersal-global" : true
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
                "comment0" : "items below this line in PlantType not currently used",
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
                "repro-seed-dispersal-global" : true
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

|Parameter name in JSON config file|Corresponding variable name in ModelParams.h|Type of variable|Description|
 |---|---|---|---|
|visualisation|m_bVisualisation|bool|Use visualisation for this run?|
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
|log-flags|m_bLogPollinatorsIntraPhaseFull, m_bLogPollinatorsInterPhaseFull, m_bLogPollinatorsInterPhaseSummary, m_bLogFlowersInterPhaseFull, m_bLogFlowersInterPhaseSummary, m_bLogFlowersIntraPhaseFull, m_bLogFlowersIntraPhaseSummary, m_bLogFlowerMPsInterPhaseSummary, m_bLogFlowerInfoInterPhaseSummary|std::string|Flags to control logging functionality. Any combination of the following flags may be listed in the string, no separator is required: Q=PollinatorsIntraPhaseFull, P=PollinatorsInterPhaseFull, p=PollinatorsInterPhaseSummary, F=FlowersInterPhaseFull, f=FlowersInterPhaseSummary, G=FlowersIntraPhaseFull, g=FlowersIntraPhaseSummary, m=FlowerMPsInterPhaseSummary, n=FlowerInfoInterPhaseSummary. See the [Output log file formats](#output-log-file-formats) section below for further information.|
|log-update-period|m_iLogUpdatePeriod|int|Number of model steps between each update of logger|
|log-dir|m_strLogDir|std::string|Directory name for logging output during a run|
|log-final-dir|m_strLogFinalDir|std::string|Directory to which to move all log files at end of run (if blank, files are kept in m_strLogDir)|
|log-run-name|m_strLogRunName|std::string|Run name to be used as prefix for log filenames|
|use-log-threads|m_bUseLogThreads|bool|Use a separate thread for writing log files?|
|verbose|m_bVerbose|bool|Should progress messages be printed on stdout?|
|sim-termination-num-gens|m_iSimTerminationNumGens|int|Terminate run after this number of generations|
|generation-termination-type|m_GenTerminationType|GenTerminationType|Method used to define termination criterion for a generation. Allowed values: num-sim-steps, num-pollinator-steps, pollinated-fraction, pollinated-fraction-all, pollinated-fraction-species1.|
|generation-termination-param|m_iGenTerminationParam or m_fGenTerminationParam|int or float|Parameter associated with m_GenTerminationType (type of param depends on value of generation-termination-type)|
|colour-system|m_ColourSystem|ColourSystem|Determines whether we are using regularly-spaced marker points to define the allowed flowers in this run (in which case the PlantType config sections can use the flower-reflectance-mp-init-min/max/step specs), or whether we are using an arbitrary (not necessarily regularly-spaced) set of dominant wavelengths (in which case the PlantType config sections must use the flower-vis-data-id spec to refer to a specific entry in the Pollinator vis-data array). Allowed values: regular-marker-points, arbitrary-dominant-wavelengths. Default value is regular-marker-points.  When regular-marker-points is specified, the vis-data array must be in the 6-value configuration, and when arbitrary-dominant-wavelengths is specified, the vis-data array must be in the 10-value configuration. See the vis-data entry in the Pollinator configuration section for further details.|
|auto-distribs|m_bPtdAutoDistribs|bool|Use auto-generation tool for Plant Type Distributions?|
|auto-distrib-num-rows |m_iPtdAutoDistribNumRows|int|PlantTypeDistribution auto-generation number of rows of areas to generate|
|auto-distrib-num-cols|m_iPtdAutoDistribNumCols|int|PlantTypeDistribution auto-generation number of columns of areas to generate|
|auto-distrib-equal-nums|m_bPtdAutoDistribEqualNums|bool|If true, allocates an equal number of patches to all defined plant types, leaving any remaining patches in the environment empty. If false, all patches are filled, so some plant types may get one more patch than others.|
|auto-distrib-density|m_fPtdAutoDistribDensity|float|PlantTypeDistribution auto-generation density of plants in each area|
|auto-distrib-area-margin|m_fPtdAutoDistribAreaMargin|float|PlantTypeDistribution auto-generation margin without flowers in each area (expressed in percentage of area's smaller side length)|
|auto-distrib-regular|m_bPtdAutoDistribRegular|bool|Are the patches distributed in a regular or stochastic pattern?|
|auto-distrib-seed-outflow-allowed|m_bPtdAutoDistribSeedOutflowAllowed|bool|Defines whether seed-outflow-allowed is set true or false for the automatically generated patches.|
|rng-seed|m_strRngSeed|std::string|Seed string used to seed RNG. This is specified as an alphanumeric string of arbitrary length, composed of digits, uppercase letters and lowercase letters.|

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
|step-type|strStepType|std::string|Determines how a pollinator moves if/when it decides to move in a given step of a foraging run. For value 'constant' it will always attempt to move a constant distance in a uniform random direction with distance defined by the parameter step-length. For value 'levy' it will attempt to move by a distance determined by a standard Cauchy distribution (which provides an inverse-square law tail) in a uniform random direction, with a minimum step of 0.5 units and maximum of 20.0 units (currently hard-wired in Pollinator::moveLevy()). In all cases, if the attempted move would take the pollinator out of its allowed foraging area, the attempted movement is reflected back into the allowed area. Allowed values: constant, levy|
|step-length|stepLength|float|For step-type=constant, step-length determines the length of a single step of the pollinator's flight during foraging|
|max-pollen-capacity|maxPollenCapacity|int|Maximum amount of pollen the pollinator can carry|
|pollen-deposit-per-flower-visit|pollenDepositPerFlowerVisit|int|Amount of pollen deposited on a flower on each visit|
|pollen-loss-in-air|pollenLossInAir|int|Amount of pollen lost on each timestep when flying|
|pollen-carryover-num-visits|pollenCarryoverNumVisits|int|After collecting a grain of pollen from a flower, the pollinator can visit this number of subsequent flowers to potentially deposit it. If the pollen is still not deposited after this number of visits, it is removed from the pollinator (i.e. it is lost). A value of 0 indicates no limit in number of visits.|
|constancy-type|strConstancyType|std::string|Defines the pollinator's constancy behaviour while foraging (NB 'simple'=if sees a flower of same species as last visited, land with probablity 0.9, if its a different species, land with probability (1.0-constancy-param). 'visual'=utilise the pollinator's full visual system and learning strategies for deciding which flowers to visit - this option should be used in conjunction with foraging-strategy='nearest-flower'). Allowed values: none, simple, visual|
|constancy-param|constancyParam|float|If constancy-type=simple, constancy-param is a number between 0.0 and 1.0. In this case, when the pollinator sees a flower of the same species it has landed on before, it lands on it with probability 0.9. If it is of a different species, it lands on it with probability (1.0-constancy-param).|
|foraging-strategy|strForagingStrategy|std::string|Defines how the pollinator forages (NB for foraging using visual system, foraging-strategy must be 'nearest-flower' and constancy-type must be 'visual'). Allowed values: random, nearest-flower, random-flower, random-global|
|learning-strategy|strLearningStrategy|std::string|Defines how the pollinator learns from experience (NB for this parameter to have effect, foraging-strategy must be 'nearest-flower' and constancy-type must be 'visual'). Allowed values: **stay**, **stay-rnd**, **stay-innate**, **fickle-circumspect**, **deliberative-decisive**, **none**. For 'stay', pollinator sets target to the first flower it lands on from which it receives a reward, and keeps it for the rest of the bout. For 'stay-rnd', pollinator is assigned a marker point at uniform random (picked from all MPs defined in the vis-data section) at start of foraging bout and keeps it for the whole bout. 'stay-innate' is like 'stay-rnd' except the initial random allocation is weighted according to innate preference data for real bees as published in Giurfa et al 1995. For 'none', pollinator never sets a target so effectively may land with equal probability on any flower it sees (use in conjunction with a high value for vis-prob-land-no-target-set-delta).|
|visited-flower-memory-size|visitedFlowerMemorySize|unsigned int|The maximum number of recently visited flowers that the pollinator can remember. This may affect whether or not it re-lands on a given flower, depending upon its foraging strategy.|
|nectar-collect-per-flower-visit|nectarCollectPerFlowerVisit|int|Amount of nectar the pollinator attempts to extract from a flower on a single visit.|
|vis-data|visData|6xN array or 10xN array|This array holds the data that specifies how the pollinator's visual system responds to a defined set of stimuli. In the JSON config file it takes the form of a 6xN array or a 10xN array, depending on whether the colour-system parameter is set to regular-marker-points or arbitrary-dominant-wavelengths, respectively.  In the 6xN case, each row contains 6 numbers representing (1) Marker Point of stimulus (int); (2) Pollinator's probability of detecting this marker point (float in range 0.0 to 1.0); (3) Activation value of pollinator's Green receptor for this marker point (float in range 0.0 to 1.0); (4) X coordinate of this marker point in hexagon colour space (float); (5) Y coordinate of this marker point in hexagon colour space (float); (6) the pollinator's base innate probability of landing on this marker point when it is not the target marker point (float in range 0.0 to 1.0) [but see also vis-prob-land-nontarget-indiv-stddev]. In the 10xN case, each row contains 10 numbers representing (1) A unique ID for this stimulus, as used by the flower-vis-data-id param of the PlantType configurations (int); (2) Dominant wavelength of this stimulus (int); (3) Pollinator's probability of detecting this stimulus (float in range 0.0 to 1.0); (4) Activation value of pollinator's Green receptor for this stimulus (float in range 0.0 to 1.0); (5) X coordinate of this stimulus in hexagon colour space (float); (6) Y coordinate of this stimulus in hexagon colour space (float); (7) X coordinate of the point where the projection of this stimulus in hex space intersects the pure spectral line (float); (8) Y coordinate of the point where the projection of this stimulus in hex space intersects the pure spectral line (float); (9) the pollinator's base innate probability of landing on this marker point when it is not the target marker point (float in range 0.0 to 1.0) [but see also vis-prob-land-nontarget-indiv-stddev]; (10) An auxiliary ID for this entry, used only for further identification purposes in output logs (e.g. this could be the FReD ID of the stimulus) (int)|
|vis-base-prob-land-target|visBaseProbLandTarget|float|Pollinator's base probability of landing on a target flower. (default: 0.9)|
|vis-prob-land-no-target-set-delta|visProbLandNoTargetSetDelta|float|If pollinator currently has no target, its innate probability of landing on a given marker point is increased by this amount. (default: 0.2)|
|vis-prob-land-nontarget-indiv-stddev|visProbLandNonTargetIndivStdDev|float|Each pollinator's innate probability of landing on a given (non-target) marker point has an element of stochasticity as determined by this parameter, which defines the standard deviation around the base value defined in the corresponding vis-data row. (default: 0.01)|
|vis-prob-land-increment-on-reward|visProbLandIncrementOnReward|float|Learning algorithm parameter for increase in probability of landing on a marker point if the current flower is rewarding. (default: 0.01)|
|vis-prob-land-decrement-on-no-reward|visProbLandDecrementOnNoReward|float|Learning algorithm parameter for decrease in probability of landing on a marker point if the current flower is not rewarding. (default: 0.01)|
|vis-prob-land-decrement-on-unseen|visProbLandDecrementOnUnseen|float|Learning algorithm parameter for decrease in probability of landing on a marker point if no flower with the marker point is currently in the pollinator's list of recently visited flowers. (default: 0.005) [see also visited-flower-memory-size]|
|vis-target-exact-match-only|visTargetExactMatchOnly|bool|If true, this overrides the normal visual discrimination code and will only match a stimulus with the target if they share the exact same marker point.|
|vis-match-min-hex-distance|visMatchMinHexDistance|float|Defines the left end of the slope on the colour hex distance axis of the piecewise linear discrimination function, such that any stimulus that is this near or nearer from the pollinator's current target stimulus will be given a confidence-of-match defined by vis-match-max-confidence.|
|vis-match-max-confidence|visMatchMaxConfidence|float|The maxconfidence of match|
|vis-match-max-hex-distance|visMatchMaxHexDistance|float|Defines the right end of the slope on the colour hex distance axis of the piecewise linear discrimination function, such that any stimulus that is this far or further away from the pollinator's current target stimulus will be given a confidence-of-match defined by vis-match-min-confidence.|
|vis-match-min-confidence|visMatchMinConfidence|float||

### Plant Type configuration parameters

Plant Type configuration parameters for a single plant type are stored in an instance of the `PlantTypeConfig` structure, defined in source file `PlantTypeConfig.h`. These are stored in the `ModelParams` member `m_PlantTypes`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeConfig.h|Type of variable|Description|
|---|---|---|---|
|species|species|std::string|Text label designating species of plant (any text is allowed)|
|flower-vis-data-id|flowerVisDataID|int|Links this plant type to visual data stored in the vis-data array in the entry with the specified ID (the first field in the vis-data entry). This parameter is required when colour-system=arbitrary-dominant-wavelengths (and is ignored otherwise).|
|flower-reflectance-mp-init-min|flowerMPInitMin|MarkerPoint|Newly created flowers of this type are randomly assigned a marker point in the range flowerMPInitMin to flowerMPInitMax with step size flowerMPInitStep within this range. This parameter (along with the corresponding -max and -step parameters) is required when colour-system=regular-marker-points (and is ignored otherwise).|
|flower-reflectance-mp-init-max|flowerMPInitMax|MarkerPoint|See flowerMPInitMin|
|flower-reflectance-mp-init-step|flowerMPInitStep|MarkerPoint|See flowerMPInitMin. Default value is 10.|
|diff-mp-is-diff-species|diffMPIsDiffSpecies|bool|If true and flowerMPInitMax != flowerMPInitMin, then plants with different flower marker points generated from this configuration are treated as different species. In this case, each individual species is given a name "PlantSpeciesNNN" (where NNN is the wavelength of the marker point) - this naming scheme overrides anything specified in the "species" field in the Plant Type configuration.|
|anther-init-pollen|antherInitPollen|int|Amount of pollen available on anther at the start of each foraging phase|
|anther-pollen-transfer-per-visit|antherPollenTransferPerVisit|int|Number of pollen grains deposited on a pollinator per visit|
|stigma-max-pollen-capacity|stigmaMaxPollenCapacity|int|Maximum amount of pollen the stigma can carry|
|pollen-clogging|pollenCloggingSpecies|std::string|Allowed values: empty string (does not clog any other species), "all" (clogs all other species), or a comma separated list of the names of the plant species that this species clogs|
|init-nectar|initNectar|int|Initial amount of nectar available in a new flower.|
|diff-mp-is-diff-species|diffMPIsDiffSpecies|bool|if true and flowerMPInitMin != flowerMPInitMax, plants created with different marker points are treated as different species (default=false)|
|repro-seed-dispersal-global|reproSeedDispersalGlobal|bool|Can seeds be dispersed at random across the whole environment?|
|repro-seed-dispersal-radius-stddev|reproSeedDispersalRadiusStdDev|float|The standard deviation of see dispersal around the parent plant, expressed in env units (1.0=one patch), ignored if reproSeedDispersalGlobal=true.|

### Plant Distribution configuration parameters

Plant distribution configuration parameters for a specified plant type in a specified region of the environment are stored in an instance of the `PlantTypeDistributionConfig` structure, defined in source file `PlantTypeDistributionConfig.h`. These are stored in the `ModelParams` member `m_PlantDists`.

|Parameter name in JSON config file|Corresponding variable name in PlantTypeDistributionConfig.h|Type of variable|Description|
|---|---|---|---|
|species|species|std::string|Name of the plant species, or one of the following: "any" or "nogo". If set to "any", then each plant in the distribution is of a randomly chosen species from all species specified in the PlantTypes section. If set to "nogo", then this is a "no go" area in which neither plants nor pollinators may be placed. For "nogo" areas, the only other parameters that need to be specified in the `PlantTypeDistribution` section are the top/left and bottom/right coordinates of the area.|
|area-top-left-x, area-top-left-y|areaTopLeft|iPos (Position\<int\>)|Top-left corner of initial distribution area|
|area-bottom-right-x, area-bottom-right-y|areaBottomRight|iPos (Position\<int\>)|Bottom-right corner of initial distribution area|
|density|density|float|Density of initial distribution (wrt specified distrib area)|
|refuge|refuge|bool|Is this area a refuge for this plant species?|
|refuge-alien-inflow-prob|refugeAlienInflowProb|float|If this is a refuge, what is the probability that an alien species attempting to germinate in this area will do so successfully?|
|seed-outflow-allowed|seedOutflowAllowed|bool|Can the plants germinate outside of the initial distrib area?|
|seed-outflow-restricted|seedOutflowRestricted|bool|If seedOutflowAllowed, is it restricted?|
|seed-outflow-prob|seedOutflowProb|float|If seedOutflowAllowed and seedOutflowRestricted, what is the probability that they are successful if attempting to do so?|
|repro-local-density-constrained|reproLocalDensityConstrained|bool|During reproduction, is the maximum density of plants in this local area constrained?|
|repro-local-density-max|reproLocalDensityMax|float|If reproLocalDensityConstrained, this is the maximum allowed density|

## Output log file formats

As shown in the [General parameters](#general-parameters) section above, there are various types of logging data that may be requested from a run. The `log-flags` parameter specifies zero, one or more flags for different kinds of output. The output from all requested flags is recording in a single log file. Each logging event appears as a separate line in the log file, and each line is a list of comma separated values (so the log file is in .csv format). The first item of every line in a single letter showing the corresponding log-flag associated with the line (e.g. 'Q', 'P', 'F', 'G', 'p', 'f', 'g', 'm', 'n') --- uppercase letters refer to full reporting formats, and lowercase letters to summary reporting formats.

To fully understand the specific format of each line, consult the corresponding methods in the `Logger` class.

A summary of some of the formats is shown below.

### log-flags=f  (Logger::logFlowersInterPhaseSummary)

 1. "f"
 2. generation number
 3. step number
 4. flower species ID
 5. flower species name
 6. number of plants of this species in this generation
 7. number of pollinated plants of this species at end of foraging phase in this generation

### log-flags=F  (Logger::logFlowersInterPhaseFull)

For each plant in the environment, emit a line with the following format at the end of each generation:

 1. "F"
 2. generation number
 3. plant unique ID
 4. plant species ID
 5. plant x position
 6. plant y position
 7. patch locality ID

These are followed by entries for each flower on the plant (NB the current implementation only allows a single flower on each plant):
 
 7. ":"
 8. flower unique ID
 9. flower pollination status ("P"=pollinated, "N"=not pollinated)
 10. flower marker point

This is followed by entries for each pollen grain in the flower's stigma:

 11. "~"
 12. pollen source flower species (marker point)
 13. number of pollen grains on stigma from this source species
 14. pollen source flower unique ID (only useful if there is only a single grain of pollen from this source species, as multiple grains may have come from multiple flowers)
 15. "~"

### log-flags=G  (Logger::logFlowersIntraPhaseFull)

For each plant in the environment, emit a line with the following format at regular intervals throughout a generation (determined by m_iLogUpdatePeriod):

 1. "G"
 2. generation number
 3. step number
 4. flower unique ID
 5. flower species ID
 6. flower x position
 7. flower y position
 8. flower marker point
 9. flower pollination status ("P"=pollinated, "N"=not pollinated)
 10. amount of pollen available on anther
 11. amount of pollen deposited on stigma 
 12. amount of nectar available

### log-flags=m  (Logger::logFlowerMPsInterPhaseSummary)

 1. "m"
 2. generation number
 3. step number
 4. marker point
 5. number of plants with this flower marker point in this generation
 6. number of pollinated plants with this flower marker point at end of foraging phase in this generation
 7. number of plants in communal regions (non-refuges) with this flower marker point in this generation
 8. number of pollinated plants in communal regions (non-refuges) with this flower marker point at end of foraging phase in this generation

### log-flags=n (Logger::logFlowerInfoInterPhaseSummary)

 1. "n"
 2. generation number
 3. step number
 4. stimulus ID (as defined by the first field of the corresponding entry in the vis-data array)
 5. number of plants of this flower stimulus type (colour) in this generation
 6. number of pollinated plants of this flower stimulus type (colour)  at end of foraging phase in this generation
 7. number of plants in communal regions (non-refuges) of this flower stimulus type (colour)  in this generation
 8. number of pollinated plants in communal regions (non-refuges) of this flower stimulus type (colour)  at end of foraging phase in this generation
 9. auxiliary ID associated with this stimulus (as defined by ninth field in vis-data entry)
 10. dominant wavelength of this stimulus (as defined by second field in vis-data entry)

### log-flags=p  (Logger::logPollinatorsInterPhaseSummary)

The first 3 fields are:

 1. "p"
 2. generation number
 3. pollinator ID

These are followed by entries for each plant species, each one comprising 4 further fields:

 4. flower species ID
 5. number of landings
 6. number of pollinations
 7. number of pollen grains of this species carried by the pollinator at end of foraging phase


### log-flags=Q  (Logger::logPollinatorsIntraPhaseFull)

 1. "Q"
 2. generation number
 3. step number
 4. pollinator species name
 5. pollinator ID
 6. pollinator x position
 7. pollinator y position
 8. pollinator heading
 9. number of flowers visited
 10. step number of most recent action
 11. flower marker point (if action=ON_FLOWER or DECLINED_FLOWER) or 0 (if action=NO_FLOWER_SEEN)
 12. reward received (if action=ON_FLOWER) or -1 (if action=DECLINED_FLOWER) or -2 (if action=NO_FLOWER_SEEN)
 13. "T" if pollinator judged flower to match its current target, otherwise "F"
 14. "//"
 15. pollinator's current target marker point
 16. "::"
 17. fields 17 onward record the pollinator's current visual preference data, in groups of three fields. The first field gives the marker point for which the following two fields apply, the second gives the probability of the pollinator landing on that marker point if it is the current target MP, and the third gives the probability of the pollinator landing on that marking point if it is not the current target MP. After these triplets have been recorded for every marker point that the pollinator knows about, the final field of the line in the log file is another "::"

 
 

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU0NDM4MTMwLDE5OTIyOTYyNTQsMTIzOD
czMjQ2LDE4NTY2NDk1ODMsLTg0OTgwNjU0NywtNzI4ODkzMzQ0
LDE3MDI5ODE0NSwtMTE1NjY3Nzk2NCw1MDg1ODUwMDIsNzQ0MT
Y4NjY4LC0xMjk4Njc3NTA4LC0xMzM0NTAwNjI2LDE5ODc0ODM3
ODksMTgxNjQzMTc0NSwtMzg2MzQ1MDM4LC0xODc5ODE5NzE0LC
04NTAyNDQ3NTgsMjA0MzM3NTYwMSwtODYzNDMzNzEwLC01NjQ1
NjM1OTZdfQ==
-->