#!/bin/sh

SDL2_TTF_VERSION=2.0.14

sudo apt-get install libfreetype6-dev
wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-${SDL2_TTF_VERSION}.zip

unzip SDL2_ttf-${SDL2_TTF_VERSION}.zip

cd SDL2_ttf-${SDL2_TTF_VERSION}

./configure --with-sdl-prefix=/usr/local --prefix=/usr/local
make
sudo make install

