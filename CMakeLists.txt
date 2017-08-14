cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(evobee VERSION 0.1.1.1 LANGUAGES CXX C)

# Specification of custom installation prefixes for required libraries
# At present, we have just added the location of the manual build of the SDL2
# library on MonARCH
#set(CMAKE_PREFIX_PATH "/home/ttay0006/evobee/code/sdl2;/home/ttay0006/evobee/code/sdl2_image")
set(CMAKE_PREFIX_PATH "/home/ttay0006/evobee/code/sdl2")
set(CMAKE_LIBRARY_PATH "/home/ttay0006/evobee/code/sdl2/lib")

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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -pthread")

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

# Configure SDL2
# Note: Using FindPkgConfig seems to be the most flexible approach.
# An alternative would be to use CMake's FindSDL module
# See http://stackoverflow.com/questions/28395833/using-sdl2-with-cmake for more info
include(FindPkgConfig)
set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# define sources
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
    src/Pollinator.cpp
    src/Visualiser.cpp
    )
# Now specify SDL2_GFX source files
set(SDL2_GFX_SRCDIR "3rd-party/sdl2-gfx/src")
file(GLOB SDL2_GFX_SOURCES "${SDL2_GFX_SRCDIR}/*.c")
# And JSON source files
set(JSON_SRCDIR "3rd-party/nlohmann-json")

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${SDL2_GFX_SRCDIR}"
    "${JSON_SRCDIR}"
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# build targets
add_executable(${PROJECT_NAME} ${SOURCES} ${SDL2_GFX_SOURCES})

# link libraries
## (TODO: is there a more generic way to link to C++ filesystem lib?)
link_directories( /home/ttay0006/evobee/code/sdl2/lib )
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${Boost_LIBRARIES} -lstdc++fs)

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


#### not used below this line ##################################
#install (TARGETS evobee DESTINATION bin)
#install (FILES "${PROJECT_BINARY_DIR}/evobeeConfig.h" DESTINATION include)
