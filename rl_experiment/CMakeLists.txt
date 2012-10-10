cmake_minimum_required(VERSION 2.4.6)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

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
	boost_system
	boost_thread
	boost_filesystem
)


#src/agent.cpp
#src/Planners/ParallelETUCT
