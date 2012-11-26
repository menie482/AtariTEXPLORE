#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#./experiment --agent texplore --explore epsilongreedy --epsilon 0.05 --rom ./games/asterix.bin
#./experiment --agent texplore --actrate 5 --nstates 50 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin
./experiment --agent texplore --actrate 5 --explore variancenovelty --n 5 --v 3 --rom ./games/asterix.bin

set +e
