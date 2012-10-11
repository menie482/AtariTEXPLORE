#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

echo "Cleaning all make/cmake files"
cd ale
make clean

cd ..
find -iname '*cmake*' -not -name CMakeLists.txt -exec rm -rf {} \+

set +e
