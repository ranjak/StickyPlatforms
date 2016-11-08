# StickyPlatforms

A basic 2D platformer to get some experience with game development.

## Requirements

SDL2, SDL2\_image, SDL2\_ttf, CMake.

A C++ compiler that supports C++11.

## Building

### Linux

```
mkdir build
cd build
cmake ..
make
```

### Windows (MSVC)

In CMake, before configuring the project, add a variable `SDL2_PATH` pointing to the root folder where SDL2, SDL2\_image  and SDL2\_ttf libraries are located
(the folder should contain `lib` and `ìnclude` subfolders).
You can then proceed to configure, generate and build the project in Visual Studio.

### Other

So far, the game has been tested on Linux and Windows only, but it should work at least on Mac as well.

## Running

When running the game, make sure your working directory is the source directory, where the `assets` folder is, otherwise the game won't find it.

### How To Play

  * __Run__: Left/Right arrow keys
  * __Jump__: Space key. How long you hold the key down will influence your jump height.
  * __Sword__: F key
  * __Climb a ledge__: Hold the direction corresponding to the ledge you are facing (or hold Space).
  * __Cling to decor__: Hold _Jump_ while in the air to cling to horizontal decor elements you come in contact with.
  * __Climb onto a platform__: While clinging to a platform, press the Up arrow key to climb on top of it.
  * __Wall Jump__: While in the air and in contact with a wall, hold the direction opposite to the wall and press _Jump_ to perform a wall jump.

## License

MIT

## Credits

Tio Aimar @ opengameart.org for the tiles.
