# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

EvoBee is an agent-based simulation platform for modeling bee pollination of crops and its evolution under changing environmental conditions. It's a C++ application with SDL visualization capabilities, developed by Tim Taylor and Alan Dorin at Monash University.

## Build System and Commands

### Building the Application
```bash
# Initial build
mkdir -p build && cd build
cmake ..
make

# Subsequent builds (from project root)
cd build && make
```

### Running the Application
```bash
# Run with full configuration
./build/Debug/evobee evobee.cfg.json

# Run with simplified test configuration
./build/Debug/evobee evobee-test-simple.cfg.json

# Run from build directory
cd build/Debug
./evobee ../../evobee.cfg.json
```

### Configuration Files
- `evobee.cfg.json` - Full simulation configuration with all parameters
- `evobee-test-simple.cfg.json` - Simplified configuration for testing
- Configuration uses JSON format with nested parameter groups (SimulationParams, Environment, Hives, etc.)

## Architecture Overview

### Core Design Pattern
EvoBee follows a **hierarchical agent-based simulation** architecture:

1. **EvoBeeModel** - Top-level simulation coordinator
   - Manages generations and simulation steps
   - Owns static RNG engine shared across all components
   - Controls the main simulation loop

2. **Environment** - Spatial simulation space
   - Manages a grid of **Patches** (spatial units)
   - Tracks all **AbstractHives** and aggregates all **Pollinators**
   - Provides spatial queries for flower finding and movement

3. **Agent Hierarchy** - Polymorphic pollinator system
   - **AbstractHive** (abstract base) → **Hive<T>** (templated) → specific hive types
   - **Pollinator** (abstract base) → **HoneyBee**, **Hymenoptera** (concrete types)
   - Uses template pattern for type-safe hive management

4. **Ecological Entities** - Plant-flower system
   - **FloweringPlant** contains multiple **Flower** objects
   - **Flower** handles pollen transfer and nectar collection
   - **Pollen** tracks species and carryover counts

### Key Architectural Patterns

**Template Pattern**: `Hive<T>` allows type-safe management of different pollinator species while maintaining polymorphic interface through `AbstractHive`.

**Strategy Pattern**: Pollinators support multiple foraging strategies (random, nearest-flower, random-flower) and movement patterns (constant, Levy flight).

**Static Factory**: `AbstractHive::makeHive()` creates appropriate hive types based on configuration.

**Component System**: All major entities inherit from `ModelComponent` for consistent lifecycle management.

### Configuration-Driven Behavior
The simulation is heavily configuration-driven through JSON files:
- **SimulationParams**: Global simulation settings, visualization, logging
- **Environment**: Grid size, reproduction constraints
- **Hives**: Pollinator types, populations, foraging areas
- **PlantTypeDistributions**: Plant species placement and properties

### Memory Management
- Uses RAII with smart pointers (`std::shared_ptr`) for hives
- Raw pointers for parent-child relationships (non-owning)
- STL containers for collections
- "Rule of 5" implemented for major classes

### Visualization System
- SDL3-based real-time visualization
- Optional trails showing pollinator movement paths
- Configurable update periods and display parameters
- Can be disabled for batch runs

### Evolutionary Simulation
- Multi-generational with plant reproduction based on pollination success
- Genetic algorithms with mutation for plant traits
- Supports refuges and seed dispersal constraints
- Species tracking with unique ID system

## Key Files to Understand

**Core Simulation**:
- `src/evobee.cpp` - Main entry point and configuration parsing
- `src/EvoBeeModel.cpp` - Main simulation loop and RNG management
- `src/Environment.cpp` - Spatial management and agent coordination

**Agent System**:
- `include/AbstractHive.h` - Hive interface and factory
- `include/Pollinator.h` - Base pollinator class with foraging strategies
- `src/HoneyBee.cpp`, `src/Hymenoptera.cpp` - Concrete pollinator implementations

**Ecological Components**:
- `src/FloweringPlant.cpp` - Plant lifecycle and species management
- `src/Flower.cpp` - Pollen transfer and nectar mechanics

**Configuration**:
- `include/ModelParams.h` - Global parameter management
- `include/PlantTypeConfig.h` - Plant species configuration
- `include/PollinatorConfig.h` - Pollinator behavior configuration

## Development Notes

When modifying the simulation:
- All random number generation must use `EvoBeeModel::m_sRngEngine`
- New pollinator types require implementing pure virtual methods in `Pollinator`
- Plant species are managed through static ID mapping in `FloweringPlant`
- Visualization updates are controlled by `vis-update-period` in configuration
- Logging flags control output verbosity: 'P'=positions, 'F'=flowers, 'p'/'f'=summaries

The codebase emphasizes scientific reproducibility - all randomness is centrally controlled and configurable via seed parameters.