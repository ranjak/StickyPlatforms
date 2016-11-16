#!/bin/sh

SDL2_IMAGE_VERSION=2.0.1

sudo apt-get install libpng-dev
wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-${SDL2_IMAGE_VERSION}.zip

unzip SDL2_image-${SDL2_IMAGE_VERSION}.zip

cd SDL2_image-${SDL2_IMAGE_VERSION}

./configure --disable-bmp --disable-gif --disable-jpg --disable-lbm --disable-pcx --disable-png-shared --disable-pnm --disable-tga --disable-tif --disable-xcf --disable-xpm --disable-xv --disable-webp --with-sdl-prefix=/usr/local --prefix=/usr/local

make
sudo make install
