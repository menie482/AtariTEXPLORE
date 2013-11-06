#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#./experiment --agent texplore --explore epsilongreedy --epsilon 0.05 --rom ./games/asterix.bin
#./experiment --agent texplore --actrate 5 --nstates 50 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin
#./experiment --agent texplore --model c45tree --nstates 20 --actrate 1 --explore variancenovelty --n 20 --v 20 --rom ./games/asterix.bin
#./experiment --agent texplore --actrate 3 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin --history 1 --model c45tree
#./experiment --agent sarsa --rom ./games/asterix.bin

#./experiment --agent texplore --planner uct --explore variancenovelty --n 5 --v 2 --rom ./games/boxing.bin --history 1 --model c45tree --nmodels 1 --combo best

#./experiment --agent texplore --explore epsilongreedy --rom ./games/asterix.bin --model c45tree --nmodels 1 --combo best --actrate 30

#./experiment --agent texplore --explore epsilongreedy --nmodels 1 --combo best --rom ./games/asterix.bin --actrate 30

#./experiment --agent texplore --rom ./games/asterix.bin --actrate 2 --n 5 --v 2 --history 1

#./experiment --agent texplore --env fuelworld

./experiment --agent qlearner --env fuelworld

set +e
