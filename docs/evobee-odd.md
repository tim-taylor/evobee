# EvoBee high level description

## Introduction

This document provides a high-level description of the EvoBee agent based simulation platform. It follows the *ODD* protocol (Overview, Design concepts, and Details) proposed by Grimm et al.[^Grimm2010] as a standardised format for the description of agent-based models.

[^Grimm2010]: http://dx.doi.org/10.1016/j.ecolmodel.2010.08.019 "The ODD protocol: A review and first update"

## Purpose

EvoBee is a spatially-explicit agent-based model designed to simulate interactions between pollinators and plant species under a variety of scenarios. It simulates a population of individual pollinators foraging an area containing one or more different plant species over the course of a season, and also changes and evolution in the plant population over the course of multiple seasons. 

## Entities, state variables, and scales

### Pollinator agents

Each  pollinator has a unique identification number. It keeps a record of its current position in the environment (a two-dimensional floating-point vector) and its current heading (a floating-point angle [0, 2π]). It has a pollen store, in which is recorded any pollen carried  after collection from visited flowers. It keeps a list of the ID numbers of the five most recently visited flowers, and at any given time it will not revisit a flower that is currently on this list. Finally, each pollinator has a foraging strategy, which is either *Forage Nearest* flower, or *Forage Any* flower. These strategies are explained in the [Process Overview](#process-overview-and-scheduling) section.

### Flower/Plant agents

Each plant belongs to a particular species, identified by a species ID number. Each plant also has a unique plant identification number. Each plant has one or more flowers. Each flower has a unique flower identification number. It also has a species identification number, to signify which plant species it belongs to. A flower keeps a record of its fixed position in the environment (a two-dimensional floating-point vector). It also keeps a record of the number of collectable pollen grains currently available in its anthers—this is initialised with a fixed number when a flower is created, and depleted when a pollinator visits the flower, when a fixed number of anther pollen grains are transferred to the pollinator’s pollen store. The flower has a stigma pollen store, recording any pollen that visiting pollinators have deposited (these may be conspecific or heterospecific grains). The only difference between flower species is the pollen type produced and required for pollination.

### Pollen agents

Each pollen grain is modelled individually, and records the individual flower that produced it, its species type, and (when travelling in a pollinator’s pollen store), the number of flowers the pollinator has visited since the grain was originally collected. Pollen cannot move independently, it must be transferred between flower agents by pollinator agents and is at any time always on a flower anther, stigma, or on the body of a pollinator.

### Spatial Units

Simulation distances are measured in perceptual distance units (pdu). We define 1 pdu to correspond to the distance over which bumblebees can detect a plant’s flowers, which is approximately 0.7m.

### Environment

In an EvoBee simulation,  plants, flowers and pollinators exist in a 2D continuous-space environment. The initial distribution of plants is defined by the `PlantTypeDistribution` section of the configuration file, and the initial distribution of pollinators is defined by the `Hives` section of the configuration file. It is possible to specify one or more areas of the environment as refuges for particular plant species. This is done in the `PlantTypeDistribution` section of the configuration file. 
Only plants of one specified species can grow in a refuge area. If during the reproduction phase of the simulation (see [Process overview and scheduling](#process-overview-and-scheduling)) a seed from a different species lands in the refuge, it has a reduced chance of survival (as defined by the `refuge-alien-inflow-prob` parameter in the configuration file).  

## Process overview and scheduling
    

### Overview of dynamics: foraging and reproduction phases

The simulation features a 2D continuous spatial environment containing a population of flowering plants of two different species. Each plant has one or more flowers that can be foraged by members of a single population of insect pollinator agents. The simulation cycles through two phases, an *insect foraging and pollination phase*, and a *plant reproduction phase*.

During the foraging and pollination phase, the pollinators forage from the fixed population of flowers, transferring pollen between flowers as they visit and, potentially, pollinating them. This phase proceeds in discrete steps until the specified termination criterion has been reached (e.g. after a specified number of steps have elapsed). The termination criterion for the foraging and pollination phase is specified by the `generation-termination-type` parameter in the configuration file. At this point, the plant reproduction phase begins.

During the plant reproduction phase, viable seeds from pollinated plants form the gene pool for a new generation of plants. All plants and pollinators from the previous generation are removed from the environment, and new plants are created based upon parents picked at random (without replacement) from the gene pool. New plants are distributed to random positions in the environment. New plants are created until all members of the gene pool have been reproduced, or until the maximum carrying capacity of the environment is reached. A new population of pollinators is then created and distributed to random starting positions in the environment. Next, a new foraging phase commences. This simulation continues to cycle until a specified number of foraging/reproduction phases have been completed (defined by the `sim-termination-num-gens` parameter in the configuration file).

### Pollinator dynamics

Each foraging phase begins with a population of pollinators of size and distribution as specified in the configuration file; they either all start at their Hive position, or distributed uniform-randomly in their initial foraging area defined in the `Hive` section of the configuration file. Each pollinator collects a supply of pollen by visiting flowers. At each time step during the foraging phase, the order in which pollinators are selected is randomised. Each pollinator executes a new iteration of its foraging behaviour in each simulation time step. A number of different pollinator foraging strategies are available, the most commonly used being: *Forage Nearest* flower (`foraging-strategy=nearest-flower` in configuration file), and *Forage Any* flower (`foraging-strategy=random-global` in configuration file). In both strategies, every pollinator maintains a list of its most recently visited individual flowers, and it will not revisit a flower that currently appears on this list. The size of the list (the pollinator's memory of recent flowers) is defined by the `visited-flower-memory-size` parameter in the configuration file. The differences between the strategies are as follows.

A pollinator following the *Forage Nearest* flower strategy  (see [Flowchart](#flowchart-for-forage-nearest-strategy) below) will move to (and land upon) the nearest flower within a search radius of 1 pdu from its current position and that does not appear on its recently visited flower  list. If no flower meets these criteria, the pollinator moves a distance of 1 pdu in a uniform-randomly chosen direction. This is not a strategy real insects use,  but if we are only interested in the relative success of flowering plants, as long as we are consistent in our approach across all simulations and we maintain our aspatial / well-mixed plant population, this simple model has been shown to be sufficient.

A pollinator following the *Forage Any* flower strategy (see [Flowchart](#flowchart-for-forage-any-strategy) below) picks a flower to visit at random from all flowers in the entire environment, excluding those on its list of recently visited flowers. 

The other available foraging strategies are `random`, and `random-flower`.  The `random` strategy involves first making a move in a random direction, then looking for a nearby flower. If a flower is found, the pollinator then moves to that if it is a visit candidate. The `random-flower` strategy involves first looking for a nearby unvisited flower from the pollinator's current position. If one is found and it is a visit candidate, the pollinator moves to a randomly selected flower from the eligible flowers found. If no flower is seen, it moves in a random direction.

Pollen transfer processes occur during each flower visit by a pollinator. These are described in a subsequent section ([Pollinator action on flowers](#pollinator-action-on-flowers])).

#### Flowchart for *Forage Nearest* strategy

The following flowchart details an individual pollinator agent's decision process when using the *Forage Nearest* foraging strategy to search for flowers. 

In the diagram `bee-pollen` refers to pollen carried on the body of the insect and potentially available for deposition on the stigma of a flower for pollination or pollen-clogging; `anther-pollen` refers to pollen carried on a flower anther and potentially available for deposition on the body of a bee.

![Flow diagram for Forage Nearest flower strategy](https://tim-taylor.github.io/evobee/img/EvoBee-Bee-Behaviour-Flowchart-FNF.png)


#### Flowchart for *Forage Any* strategy

The following flowchart details an individual pollinator agent's decision process when using the *Forage Any* foraging strategy to search for flowers. 

In the diagram `bee-pollen` refers to pollen carried on the body of the insect and potentially available for deposition on the stigma of a flower for pollination or pollen-clogging; `anther-pollen` refers to pollen carried on a flower anther and potentially available for deposition on the body of a bee.

![Flow diagram for Forage Nearest flower strategy](https://tim-taylor.github.io/evobee/img/EvoBee-Bee-Behaviour-Flowchart-FAW.png)


### Flower dynamics

At the start of a foraging phase, each flower begins with a fixed amount of anther pollen available for collection. Each flower also begins with a stigma free from pollen, but as the foraging phase progresses visiting pollinators may deposit pollen on its stigma. Pollinators may be allowed to deposit pollen from a different species onto the stigma (potentially pollen-clogging the flower) depending upon the configuration of the experiment. The `stigma-max-pollen-capacity` parameter specifies the capacity for pollen grains on a stigma. At the end of a foraging phase, every conspecific grain of pollen on the stigma forms a viable seed that enters the gene pool for the next generation of plants as described above ([Overview of dynamics](#overview-of-dynamics-foraging-and-reproduction-phases)). 

### Pollinator action on flowers

Each pollen grain carried by a pollinator visiting a flower is considered for transfer to the flower’s stigma subject to these limiting factors: the pollinator has a fixed maximum number of grains that it can transfer to the stigma during a visit (`pollen-deposit-per-flower-visit`); the stigma has a fixed maximum capacity for accepting pollen grains (`stigma-max-pollen-capacity`), and the stigma may or may not be susceptible to receiving pollen grains from a different species (above, section Flowers). Having taken all of these factors into account, if any pollen grains on the pollinator are eligible for transfer, then the determined number are picked at random from the pollinator for transferral to the stigma. Finally, the flower may transfer some of its anther pollen onto the pollinator’s body. Flower anthers start with a specified amount of pollen in each foraging phase; any available pollen on the anther is transferred to the pollinator, up to the fixed maximum transfer limit per visit.

##  Design concepts
    

### Basic principles

Our simulation builds upon the earlier simulation work of Waser (1978). One notable difference between our software and his is our use of a continuous spatial environment to avoid potential artefactual effects arising from modelling pollinator movements on a discrete grid [REFS: Bonnell et al 2016; Birch et al 2007]. Modern computing environments also allow us to simulate much larger environments than those that were feasible in 1978. We model up to 16,000 plants and 400 pollinators, running for up to 1000 generations, with each experimental condition repeated in 100 replicates (more in some cases), in contrast to 100 plants, 1 pollinator and up to 400 generations with 20 replicates in the early study. We have determined experimentally that the values we employ are sufficient to: confirm that our software’s behaviour matches the behaviour of Waser’s simulations; and, robustly generate the system behaviour we require to explore our own hypotheses (see section  Testing (Verification and Validation)). The novel results we report here arise from experimental configurations that were not considered in Waser’s original paper.
    

### Emergence

The key emergent property of interest in these experiments is whether or not both plant species X and Y can sustain viable coexistent populations for a given experimental configuration, or whether one species reaches fixation with the other dying out. We are primarily interested in how the pollen-clogging configuration (symmetric clogging, asymmetric clogging, or symmetric non-clogging) affects species coexistance.

### Pollinator perception

Under the default *Forage Nearest* flower strategy, pollinators can see flowers within 1 perceptual distance unit (pdu) of their current position, and move to the nearest flower seen within that distance  (Section,  Spatial Units).

Under the alternative *Forage Any* flower strategy, pollinators  move to a random flower anywhere in the entire environment. This strategy simulates an aspatial, well-mixed population of plants—it does not represent a real-world situation, but is of interest to indicate the underlying dynamics of the system.

Pollinators maintain a list of the last 5 individual flowers they have visited, and will not revisit a flower while it is on this list. The reader is reminded that the simulated insect pollinators are not flower constant. They will land on a target flower, as directed by the FN or FA strategy,  paying no heed of flower species.

### Interaction

The interactions between agents in the simulation are those between an individual pollinators, pollen and individual flowers. These interactions take place when the pollinator lands on a flower, as described in the Pollinator action on flowers section. There are no direct pollinator-pollinator or flower-flower interactions. It is possible for multiple pollinators to land and interact with  a  single flower within a time step. The order of their interactions is randomised.

### Stochasticity

The stochastic elements of the simulation are: placement of flowers and pollinators at the start of each foraging phase; the order in which pollinators are processed at each time step during the foraging phase; the direction a pollinator move if it doesn’t find a suitable nearby flower target in the FN strategy; the choice of target flower in the FA strategy; the choice of which individual pollen grains are transferred from a pollinator to a flower’s stigma upon landing; and, in the reproduction phase, the order in which viable seeds are considered for producing plants for the next generation.

### Observation

In these experiments, the main data recorded during experiments are, at the end of each generation, the generation number and the number of plants of each species X and Y currently in existence. For each experiment, we also record the full configuration set-up.

## Initialization

Full details of the configuration options available for EvoBee simulations are provided in the [EvoBee simulation configuration and execution](evobee-config.md) page.

## References

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTA0MDc5MzM4NSwxNzkyNzczNzE1LC03Nj
E4ODAxOCwtMTEzOTIzMjYxMCwtMTYyMjg0OTcwNSwtMjA2NjIw
NzY4NywtMTgyNTMzMzAyOCwyMDg4NTU5MjYzLC00MTkxNzQwOT
UsMTA4MDEwODcwNCwxMTk1OTM3ODAsLTYwMDYxNjQzMiwtMTY0
MTM4MTI0OSwtMTA5NzU2OTcxMSwtMTA3Njg1ODgwNCwxMDA0Nj
MyMjE3LDEyMjg3NTMzNDcsNzMyNTI1MTczLC00NzQ4MDI3NDYs
ODQ3MDYwMTU4XX0=
-->