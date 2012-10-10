cmake_minimum_required(VERSION 2.4.6)


include_directories(
rl_common/include
rl_env/include
rl_agent/include
ale/src
)

link_directories(
	/home/cdonahue/research/svn/standalone/ale
)

add_subdirectory(rl_common)
add_subdirectory(rl_agent)
add_subdirectory(rl_env)
add_subdirectory(rl_experiment)
