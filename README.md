Wumpus-OGL
==========

A Wumpus AI in OpenGL

This is an Implementation of the Wumpus world
with an agent ai.

The agent seeks the treasure and the wumpus,
and after grabbing the treasure and killing the wumpus returns
to its starting position to leave the world.

# Controls
f     - activate/deactivate mouse look / cursor

l     - reset camera position

[ESC] - Exit program

# Command line arguments
wumpus_ogl "path/to/worldfile" - loads a custom world

# Creating Worlds

Worlds are organized with JSON text files.
Exemplary worlds are in ./saved_worlds.

In a world save there has to be:
* one agent start position
* one wumpus position
* one treasure position
* variable count of pit positions

==========

# Used Libraries

For Texture loading LodePNG is used and included: http://lodev.org/lodepng/

Additionally is the JSON Format used for World loading http://json.org/
For that the JsonCpp library is used and included


It uses GLFW3 and glew as Submodules from:
https://github.com/glfw/glfw
https://github.com/omniavinco/glew-cmake


Other tools and libs which are necessary:
- cmake
- make
- g++ 
- libx11-dev
- libxi-dev
- libgl1-mesa-dev
- libglu1-mesa-dev
- libxrandr-dev
- libxext-dev


