#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

echo "Building ALE"
cd ale
make
cd ..

cmake -i
make

set +e
