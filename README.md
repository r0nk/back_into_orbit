#[Back into Orbit](http://www.backintoorbit.com):
Back into Orbit is a simple isometric shooter roguelike.

[Imgur](http://i.imgur.com/jAo51yo.gifv)

###Dependencies:

* SDL2
* GLEW
* OpenGL 2.0+
* Assimp (Open Asset Import Library)

###Building:

In order to properly build on a Linux system:

```
$ generate_layouts.sh > src/layouts.c
```

Then run `make` to begin compilation.

If building for Windows, you also have to have `SDL2.dll` in the directory in order to build.
