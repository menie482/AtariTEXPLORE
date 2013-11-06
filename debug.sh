#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

gdb --args experiment --agent texplore --actrate 1 --nstates 20 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin

#gdb --args experiment --agent texplore --env fuelworld

#gdb --args experiment --agent sarsa --env fuelworld

set +e
