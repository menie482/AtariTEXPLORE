cmake_minimum_required(VERSION 2.4.6)

#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
	include
	src
)

add_library(agentlib
  src/Agent/DiscretizationAgent.cc 
  src/Agent/QLearner.cc
  src/Agent/ModelBasedAgent.cc
  src/Agent/SavedPolicy.cc
  src/Agent/Dyna.cc
  src/Agent/Sarsa.cc
  src/Models/FactoredModel.cc
  src/Models/M5Tree.cc
  src/Models/LinearSplitsTree.cc
  src/Models/C45Tree.cc
  src/Models/Stump.cc
  src/Models/MultipleClassifiers.cc
  src/Models/ExplorationModel.cc
  src/Models/RMaxModel.cc 
  src/Models/SepPlanExplore.cc 
  src/Planners/ValueIteration.cc
  src/Planners/PolicyIteration.cc
  src/Planners/PrioritizedSweeping.cc 
  src/Planners/ETUCT.cc
  src/Planners/PO_ParallelETUCT.cc
  src/Planners/ParallelETUCT.cc
  src/Planners/PO_ETUCT.cc
  src/Planners/MBS.cc
  src/newmat/newmat1.cc 
  src/newmat/newmat2.cc 
  src/newmat/newmat3.cc 
  src/newmat/newmat4.cc 
  src/newmat/newmat5.cc 
  src/newmat/newmat6.cc 
  src/newmat/newmat7.cc 
  src/newmat/newmat8.cc 
  src/newmat/newmatex.cc 
  src/newmat/bandmat.cc 
  src/newmat/submat.cc 
  src/newmat/myexcept.cc 
  src/newmat/cholesky.cc 
  src/newmat/evalue.cc 
  src/newmat/fft.cc 
  src/newmat/hholder.cc 
  src/newmat/jacobi.cc 
  src/newmat/newfft.cc 
  src/newmat/sort.cc 
  src/newmat/svd.cc 
  src/newmat/newmatrm.cc 
  src/newmat/newmat9.cc
)

target_link_libraries(agentlib
	boost_system
	boost_thread
	pthread
)
