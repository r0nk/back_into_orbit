#Introduction:
Back into Orbit is a simple roguelike, currently running on linux, soon to be ported to windows.

###Dependencies:
SDL2
GLEW
OpenGL 2.0+

###BUILDING:
In order to properly build on a linux system:   
$ src/generate_layouts.sh > src/layouts.c   
then run "make" to begin compilation.   
If building for Windows, you also have to have SDL2.dll in the directory in order to build.
