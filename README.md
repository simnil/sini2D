# sini2D

sini2D is a simple library I'm working on for fun and as an excuse to learn more
about things like OpenGL and SDL2, as well as a set of utilities for other
smaller side-projects. It wraps some SDL2 functionality, allows for simple 2D
graphics and provides a few mathematical objects like vectors and matrices. I
plan to also add more CUDA compatibility.

It is obviously __very much a work in progress__, lacking many features and will
probably be replaced or split up in the future. Feel free to look at it, build
it and see if you can learn from my mistakes. I strongly suggest not using this
library for anything serious.


## Dependencies

__CMake__ _(version 3.11 or greater)_, __GLEW__ and __SDL2__ are required for
building sini2D.

On Windows, an environment variable named `SDL2` pointing to the root of the SDL
installation directory is required for finding it.


## Building

When building sini2D, it is strongly recommended to do an out of source build,
e.g. in a subdirectory `build` directly under the project root (ignored by
git). Any examples in this section are assumed to be run from
`<sini2D-root>/build` and are written as if run from a UNIX terminal, or Git
BASH on Windows. If using e.g. powershell, replace `-<option>` with `/<option>`.

Supported `CMAKE_BUILD_TYPE`'s are:

- `Release`
- `Debug`
- _Unspecified build type_ (But not recommended)

To also build the library tests, set the variable `SINI_2D_BUILD_TESTS` to
`TRUE`. For example, to build a release version with tests run:

	cmake .. -DCMAKE_BUILD_TYPE=Release -DSINI_2D_BUILD_TESTS=TRUE

When building a solution for Visual Studio on Windows, add `-G "Visual Studio 15
2017 Win64"`. _Note: At least Visual Studio 2017 is required, since the
compiler must support the c++17 standard._
