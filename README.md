# Game of Life GL

A simple implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
in C11 and OpenGL 3.3 Core.

## Building

Clone the repository:
```
git clone git@github.com:theultimat/game-of-life-gl.git
cd game-of-life-gl
git submodule init
git submodule update
```
This project pulls in [GLFW](https://www.glfw.org/) and [GLAD](https://glad.dav1d.de/)
so make sure to initialise submodules.

Build the project using CMake:
```
mkdir build
cd build
cmake ..
make -j8
```
Run the binary:
```
./gol
```

## Options

The `gol` binary supports a number of optional command line arguments:
- `--zoom <float>`: Sets the zoom level for the viewport. By default each cell
is a single pixel which may be too small. Must be `>= 1.0f`, defaults to `1.0f`.
- `--tick-rate <float|"unlocked">`: Sets the number of ticks per second for the
simulation, defaults to `4.0f`. If a float value is provided it must be `> 0.0f`.
The string `"unlocked"` can also be used which removes the fixed tick rate and
allows the simulation to run as fast as the hardware allows.
- `<path>`: Path to an ASCII [PBM](https://en.wikipedia.org/wiki/Netpbm) file
describing the initial state. Some simple examples can be found in the `pbm`
directory. If no path is provided or the value is `--random` the cells will be
randomly initialised.

