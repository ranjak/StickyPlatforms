#!/bin/sh

SDL2_VERSION=2.0.5

# Prerequisites
sudo apt-get install libgl1-mesa-dev
wget https://libsdl.org/release/SDL2-${SDL2_VERSION}.zip

unzip SDL2-${SDL2_VERSION}.zip

cd SDL2-${SDL2_VERSION}
./configure --prefix=/usr/local --disable-audio --disable-atomic --disable-joystick --disable-haptic --disable-power --disable-threads
make
sudo make install
