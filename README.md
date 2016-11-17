# StickyPlatforms

A basic 2D platformer to get some experience with game development.

## Requirements

SDL2, SDL2\_image, SDL2\_ttf, CMake.

A C++ compiler that supports C++11.

## Building

### Linux

Make sure you have installed development libraries for SDL2, SDL2\_image and SDL2\_ttf.

Then, for a standard in-source build, navigate to the source tree and run:

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path to a destination folder for the game> ..
make
make install
```

You need to install the game so that it can load the resources it needs.
Installation destination does not matter, you can set the install prefix to anywhere you want.

### Windows (MSVC)

In CMake, before configuring the project, add a variable `CMAKE_PREFIX_PATH` pointing to the root folder where SDL2, SDL2\_image  and SDL2\_ttf libraries are located
(the folder should contain `lib` and `include` subfolders).

Set `CMAKE_INSTALL_PREFIX` to the directory where you want the game to be installed.

You can then proceed to configure, generate the project, and build the `INSTALL` target in Visual Studio.

### Other

So far, the game has been tested on Linux and Windows only, but it should work at least on Mac as well.

## How To Play

  * __Run__: Left/Right arrow keys
  * __Jump__: Space key. How long you hold the key down will influence your jump height.
  * __Sword__: F key
  * __Climb a ledge__: Hold the direction corresponding to the ledge you are facing (or hold Space).
  * __Cling to decor__: Hold _Jump_ while in the air to cling to horizontal decor elements you come in contact with.
  * __Climb onto a platform__: While clinging to a platform, press the Up arrow key to climb on top of it.
  * __Wall Jump__: While in the air and in contact with a wall, hold the direction opposite to the wall and press _Jump_ to perform a wall jump.
  
  * __Pause__: Escape
  * __Toggle fullscreen__: Alt + Return
  * __Quit__: Standard quit shortcut (Alt + F4 / Cmd + Q)

## License

MIT

## Credits

Tio Aimar @ opengameart.org for the tiles.
