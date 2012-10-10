cmake_minimum_required(VERSION 2.4.6)

#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(
include
)

add_library(envlib
	src/Env/Arcade.cc
  src/Env/tworooms.cc
  src/Env/taxi.cc
  src/Env/MountainCar.cc
  src/Env/FuelRooms.cc
  src/Env/CartPole.cc
  src/Env/fourrooms.cc
  src/Env/RobotCarVel.cc
  src/Env/energyrooms.cc
  src/Env/gridworld.cc
  src/Env/stocks.cc
  src/Env/LightWorld.cc
)

target_link_libraries(envlib
	ale
)
