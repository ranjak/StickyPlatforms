#!/bin/sh

# This script will build StickyPlatforms, as well as build/install its dependencies, on Ubuntu 12.04.
# It will then package them into an AppImage file.

WORKING_DIR=`pwd`

ARCH="x86_64"

DEPS="/usr/local/lib/libSDL2-2.0.so.0 \
/usr/local/lib/libSDL2_image-2.0.so.0 \
/usr/local/lib/libSDL2_ttf-2.0.so.0 \
/lib/$ARCH-linux-gnu/libpng12.so.0 \
/usr/lib/$ARCH-linux-gnu/libfreetype.so.6 \
/lib/$ARCH-linux-gnu/libz.so.1"

# Get and build SDL2 (unavailable on Precise)
./build_sdl2.sh
./build_sdl2_image.sh
./build_sd2_ttf.sh

# Get required dev tools
sudo apt-add-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-6 git cmake

# Get and build StickyPlatforms
git clone https://github.com/ranjak/stickyplatforms

cd stickyplatforms
mkdir -p build/StickyPlatforms.AppDir
cd build
CC=gcc-6 CXX=g++-6 cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/StickyPlatforms.AppDir/usr -DSTICKYPLATFORMS_STATIC_GLIBCXX=ON ..
make install

# Copy dependencies over to the AppDir
cd StickyPlatforms.AppDir/usr
mkdir lib
cp $DEPS lib

strip lib/* bin/*

# Get AppImage and finalize AppDir
cd ..
wget https://github.com/probonopd/AppImageKit/releases/download/continuous/AppRun
chmod a+x AppRun

cp ../../appImage/StickyPlatforms.desktop ../../appImage/stickyplatforms.png .

cd ..
wget https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage StickyPlatforms.AppDir

echo "Build succeeded! Products in `pwd`"
