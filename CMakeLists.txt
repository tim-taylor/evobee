cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(evobee VERSION 1.1.0.0 LANGUAGES CXX C)

# The following lines query the current git branch and commit hash.
# Later in this file we make these available to the C++ code by using configure_file()
#
# Get the current working branch
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Get the latest abbreviated commit hash of the working branch
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# The following include is just for integration with the Visual Studio Code editor
# It is specified as optional, so the rest of the build process will continue as
# normal if this isn't available on your system
include(CMakeToolsHelpers OPTIONAL)

# Set a default build type. You can override this on the command line as follows:
#  cmake -D CMAKE_BUILD_TYPE=Release ..
# Avaiable build types are [Debug|Release|MinSizeRel|RelWithDebInfo]
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

# Specify flags for specific build types
## (TODO is there a more generic way to do this?)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++1z -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++1z -pthread")

# We can also make our own build targets, e.g.
#SET(CMAKE_BUILD_TYPE distribution)
#SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
#SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")

# configure a header file to pass some of the CMake settings to the source code
configure_file(
    "${PROJECT_SOURCE_DIR}/include/evobeeConfig.h.in"
    "${PROJECT_SOURCE_DIR}/include/evobeeConfig.h"
)

# Configure Boost libraries
# Require at least the version of boost specified in the following line, and
# fail if this is not found
find_package(Boost 1.32.0 REQUIRED COMPONENTS program_options)

# Configure SDL and SDL_image libraries
#
# (NB the SDL3_gfx library does not come with a standard CMake configuration.
# As it is only a small library, the easiest thing to do was to add its source
# and header files directly to the main project below, rather than adding
# it as a library)

## First set the output directory for built objects.
## This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

## Now add the libraries
add_subdirectory(3rd-party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(3rd-party/SDL_image EXCLUDE_FROM_ALL)

# Add the JSON library
add_subdirectory(3rd-party/json EXCLUDE_FROM_ALL)

# Define sources of main project
# First explicitly specify all source files in the evobee project
set(SOURCES
    src/AbstractHive.cpp
    src/Colour.cpp
    src/evobee.cpp
    src/Environment.cpp
    src/EventManager.cpp
    src/EvoBeeExperiment.cpp
    src/EvoBeeModel.cpp
    src/Flower.cpp
    src/FloweringPlant.cpp
    src/HoneyBee.cpp
    src/Hymenoptera.cpp
    src/Logger.cpp
    src/ModelComponent.cpp
    src/ModelParams.cpp
    src/Patch.cpp
    src/PlantTypeConfig.cpp
    src/Pollinator.cpp
    src/ReflectanceInfo.cpp
    src/tools.cpp
    src/Visualiser.cpp
    3rd-party/SDL3_gfx/SDL3_framerate.c
    3rd-party/SDL3_gfx/SDL3_gfxPrimitives.c
    3rd-party/SDL3_gfx/SDL3_imageFilter.c
    3rd-party/SDL3_gfx/SDL3_rotozoom.c
    )

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "3rd-party/SDL3_gfx"
)

# build targets
add_executable(${PROJECT_NAME} ${SOURCES})

# set up including and linking to related libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options nlohmann_json::nlohmann_json SDL3_image::SDL3_image SDL3::SDL3)


# specify compiler features
# Approach 1: directly set compiler flags (assumes a specific compiler)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Approach 2: specify general features required (preferred)
#  See https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
#  for list of available features
target_compile_features(evobee
    PUBLIC
        cxx_auto_type
        cxx_range_for
    #PRIVATE
    #    cxx_variadic_templates
)
