Wumpus-OGL
==========

A Wumpus game in OpenGL

# Controls
f     - activate/deactivate mouse look / cursor
l     - reset camera position
[ESC] - Exit program

# command line arguments
wumpus_ogl "path/to/worldfile" - loads a custom world

# Used Libraries

For Texture loading LodePNG is used and included: http://lodev.org/lodepng/

Additionally is the JSON Format used for World loading http://json.org/
For that the JsonCpp library is used and included


It uses GLFW3 and glew as Submodules from:
https://github.com/glfw/glfw
https://github.com/omniavinco/glew-cmake


Other tools and libs which are necessary:
cmake
make
g++
libx11-dev
libxi-dev
libgl1-mesa-dev
libglu1-mesa-dev
libxrandr-dev
libxext-dev


