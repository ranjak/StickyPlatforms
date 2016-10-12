# game1

A basic 2D platformer to get some experience with game development.

## Requirements

SDL2, SDL2\_image, CMake.

A C++ compiler that supports C++11.

## Building

```
mkdir build
cd build
cmake ..
make
```

So far, the game has been tested on Linux only, but it should work at least on Windows and Mac as well.

## Running

When running the game, make sure your working directory is the source directory, where the `assets` folder is, otherwise the game won't find it.

Use the arrow keys to move around, and Space to jump. Use 'C' to swing your sword.
You're the green block, the enemy is the red one. Beware, as he will hurt you on contact. Hit him with your sword to defend yourself!

## License

MIT

## Credits

Tio Aimar @ opengameart.org for the tiles.
