cmake_minimum_required(VERSION 2.4.6)

#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(include)

add_library(commonlib
	include/rl_common/Random.cc
)
