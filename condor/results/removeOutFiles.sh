#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

find . -name "*.out.*" -type f|xargs rm -f

set +e
