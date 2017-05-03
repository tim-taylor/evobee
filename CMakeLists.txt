cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(evobee VERSION 0.1.1.1 LANGUAGES CXX C)

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
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# define sources
# Approach 1: manual addition (preferred)
set(SOURCES
    src/evobee.cpp
    src/Environment.cpp
    src/EventManager.cpp
    src/EvoBeeExperiment.cpp
    src/EvoBeeModel.cpp
    src/FloweringPlant.cpp
    src/Hive.cpp
    src/Logger.cpp
    src/ModelComponent.cpp
    src/ModelParams.cpp
    src/Patch.cpp
    src/Pollinator.cpp
    src/Visualiser.cpp
    )
# Approach 2: automatic addition
#file(GLOB SOURCES "src/*.cpp")
set(SDL2_GFX_SRCDIR "3rd-party/sdl2-gfx/src")
file(GLOB SDL2_GFX_SOURCES "${SDL2_GFX_SRCDIR}/*.c")

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${SDL2_GFX_SRCDIR}"
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# build targets
add_executable(${PROJECT_NAME} ${SOURCES} ${SDL2_GFX_SOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${Boost_LIBRARIES})

# specify compiler features
# Approach 1: directly set compiler flags (assumes a specific compiler)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Approach 2: specify general features required (preferred)
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
