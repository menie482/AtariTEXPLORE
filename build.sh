#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

echo "Building ALE"
cd ale
make -j4
cd ..

echo -ne '\n' | cmake -i -Wno-dev
make -j4

set +e
