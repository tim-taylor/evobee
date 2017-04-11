cmake_minimum_required(VERSION 3.2)
project(evobee VERSION 0.1.1.1 LANGUAGES CXX)

# configure a header file to pass some of the CMake settings to the source code
configure_file(
    "${PROJECT_SOURCE_DIR}/src/evobeeConfig.h.in"
    "${PROJECT_BINARY_DIR}/evobeeConfig.h"
)

# set any required compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Configure SDL2
# Note: Using FindPkgConfig seems to be the most flexible approach.
# An alternative would be to use CMake's FindSDL module
# See http://stackoverflow.com/questions/28395833/using-sdl2-with-cmake for more info
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# define sources
# Approach 1: manual addition
#   set(SOURCES src/evobee.cpp)
# Approach 2: automatic addition
file(GLOB SOURCES "src/*.cpp")

# build targets
add_executable(${PROJECT_NAME} ${SOURCES})

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
)

# link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})


#### not used below this line
#install (TARGETS evobee DESTINATION bin)
#install (FILES "${PROJECT_BINARY_DIR}/evobeeConfig.h" DESTINATION include)
