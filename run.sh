#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#./experiment --agent texplore --explore epsilongreedy --epsilon 0.05 --rom ./games/asterix.bin
#./experiment --agent texplore --actrate 5 --nstates 50 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin
#./experiment --agent texplore --model c45tree --nstates 20 --actrate 1 --explore variancenovelty --n 20 --v 20 --rom ./games/asterix.bin
./experiment --agent texplore --model c45tree --actrate 20 --rom ./games/asterix.bin

set +e
