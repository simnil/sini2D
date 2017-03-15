# sini2D

sini2D is a simple library adding functionality for visualizing simulations, plotting results and creating other applications with simple 2D graphics. It uses SDL2 and OpenGL and contains a few mathematical objects, like vectors and matrices, as well as related mathematical functions and some more general ones. It also allows for some CUDA compatibility.

It is obviously very much still a work in progress and will probably be replaced or split into more specialized, better libraries as it grows and is improved. I suggest not using this library in its current state.


## Building

If anyone wants to play around with it (and as a note to myself), it is recommended to build the solution with cmake from a subfolder `build` as it will be ignored by git. To build a solution for Visual Studio for windows use (replace -G with /G if building from powershell or similar):

	cmake .. -G "Visual Studio 14 2015 Win64"

Requirements:
- __CMake__
- __SDL2__

### Installing dependencies

Download and install SDL2 development libraries for Visual C++ from the official website. Then create an environment variable called SDL2 and point it to the root of the SDL2 installation.