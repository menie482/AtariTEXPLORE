#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

export PATH=/lusr/opt/condor/bin:$PATH
condor_submit condor/1.29.13.asterix.3
condor_submit condor/1.29.13.asterix.4
condor_submit condor/1.29.13.asterix.5

set +e
