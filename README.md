# sini2D

sini2D is a simple library adding functionality for visualizing simulations,
plotting results and creating other applications with simple 2D graphics. It
uses SDL2 and OpenGL and contains a few mathematical objects, like vectors and
matrices, as well as related mathematical functions and some more general
ones. It also allows for some CUDA compatibility.

It is obviously __very much still a work in progress__, lacking many features,
and will probably be replaced or split into more specialized, better libraries
as it grows and is improved. I suggest not using this library in its current
state.


## Dependencies

__CMake__ _(version 3.0 or greater)_ and __SDL2__ are required for building
sini2D.

On Windows, an environment variable named `SDL2` pointing to the root of the SDL
installation directory is required for finding it.


## Building

When building sini2D, it is strongly recommended to do so from a subdirectory
`build`, directly under the project root. Any examples in this section are
assumed to be run from `<sini2D-root>/build` and are written as if run from a
UNIX terminal, or Git BASH on Windows. If using e.g. powershell, replace
`-<option>` with `/<option>`.

Supported `CMAKE_BUILD_TYPE`'s are:

- `Release`
- `Debug`
- _No build type_ (But not recommended)

To also build the library tests, set the variable `SINI_2D_BUILD_TESTS` to
`TRUE`. For example, to build a release version with tests run:

	cmake .. -DCMAKE_BUILD_TYPE=Release -DSINI_2D_BUILD_TESTS=TRUE

When building a solution for Visual Studio on Windows, add `-G "Visual Studio 14
2015 Win64"`.
