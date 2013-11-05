#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#cd archives
#cp *.tar* ../
#cd ..

tar xvfz SDL-1.2.15.tar.gz 
tar xvfz SDL_gfx-2.0.24.tar.gz 
tar xvfz SDL_image-1.2.12.tar.gz

cd SDL-1.2.15
./configure
make
cd ..

cd SDL_gfx-2.0.24
autoreconf
./configure
make
cd ..

cd SDL_image-1.2.12
./configure
make
cd ..

set +e
