#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

echo "Building ALE"
cd ale
make
cd ..

echo -ne '\n' | cmake -i -Wno-dev -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-pg" -DCMAKE_EXE_LINKER_FLAGS_DEBUG="-pg"
make

gdb --args ./experiment --agent texplore --actrate 1 --nstates 20 --explore variancenovelty --n 5 --v 2 --rom ./games/asterix.bin

set +e
