Wumpus-OGL
==========

A Wumpus AI in OpenGL
-------

This is an Implementation of the Wumpus world
with an agent ai and a 3D environment.

The agent seeks the treasure and the wumpus,
and after grabbing the treasure and killing the wumpus returns
to its starting position to leave the world.

### OpenGL Version
OpenGL version 3.3 is needed at least.

The Shaders are also written for GLSL version 330 core.

Keyboard Controls
-------

[f]   - Activate/deactivate mouse look / cursor

[l]   - Reset camera position

[ESC] - Exit program

Command line arguments
-------

wumpus_ogl "path/to/worldfile" - loads a custom world

Creating worlds
-------

Worlds are organized with JSON text files.
Exemplary worlds are in ./saved_worlds.

In a world save there has to be:
* one agent start position (has to be on (1,1) at the moment)
* one wumpus position
* one treasure position
* variable count of pit positions

Building on Linux
-------

Required tools

- CMake
- make
- g++ 

Required libraries
- libx11-dev
- libxi-dev
- libgl1-mesa-dev
- libglu1-mesa-dev
- libxrandr-dev
- libxext-dev

On Ubuntu
- xcursor-dev
- libxcursor-dev
might also be needed

Building on Windows
-------

Required tools

- MinGW-w64
- CMake

ES Shader
-------

To use the es shaders instead of the core shaders, change the option with ccmake/cmake-gui.

Other used libraries
-------

It uses [GLFW3](https://github.com/glfw/glfw) and [glew](https://github.com/omniavinco/glew-cmake) as Submodules.



For texture loading [LodePNG](http://lodev.org/lodepng/) is used and included.

Additionally is the [JSON Format](http://json.org/) used for world loading.
For that the amalgamated version of [JsonCpp](https://github.com/open-source-parsers/jsoncpp) library is used and included.
