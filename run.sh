#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#./experiment --agent texplore --explore epsilongreedy --epsilon 0.05 --rom ./games/asterix.bin
./experiment --agent texplore --rom ./games/boxing.bin

set +e
