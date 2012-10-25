#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

./experiment --agent texplore --rom ./games/asterix.bin

set +e
