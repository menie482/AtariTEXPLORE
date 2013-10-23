cmake_minimum_required(VERSION 2.4.6)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

#IF (BUILD_CONDOR_EXE)
#    SET (CONDOR_COMPILE "condor_compile  ")
#    SET (CMAKE_CXX_COMPILER "${CONDOR_COMPILE}${CMAKE_CXX_COMPILER}")
#ENDIF (BUILD_CONDOR_EXE)

# basically add -g
set (CMAKE_BUILD_TYPE Debug)

set (CMAKE_MODULE_LINKER_FLAGS "-ldl")
set (CMAKE_SHARED_LINKER_FLAGS "-ldl")

add_library(SDL STATIC IMPORTED)
set_target_properties(SDL PROPERTIES
  IMPORTED_LOCATION ale/liblocal/SDL-1.2.15/build/.libs/libSDL.a)

add_library(SDL_image STATIC IMPORTED)
set_target_properties(SDL_image PROPERTIES
  IMPORTED_LOCATION ale/liblocal/SDL_image-1.2.12/.libs/libSDL_image.a)

add_library(SDL_gfx STATIC IMPORTED)
set_target_properties(SDL_gfx PROPERTIES
  IMPORTED_LOCATION ale/liblocal/SDL_gfx-2.0.24/.libs/libSDL_gfx.a)

find_package(DirectFB)

include_directories(
rl_common/include
rl_env/include
rl_agent/include
ale/src
ale/liblocal/include
${DIRECTFB_INCLUDE_DIR}
)

link_directories(
	ale
)

add_subdirectory(rl_common)
add_subdirectory(rl_agent)
add_subdirectory(rl_env)

add_executable(experiment
  src/rl.cc
)

target_link_libraries(experiment
    commonlib
    agentlib
    envlib
    ale
    SDL
    SDL_image
    SDL_gfx
    ${DIRECTFB_LIBRARY}
    #directfb
    m
    z
    boost_system
    boost_thread
    boost_filesystem
)
