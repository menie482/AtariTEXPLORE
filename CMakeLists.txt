cmake_minimum_required(VERSION 2.4.6)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

include_directories(
rl_common/include
rl_env/include
rl_agent/include
ale/src
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
	boost_system
	boost_thread
	boost_filesystem
)
