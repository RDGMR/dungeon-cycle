## Building

### Web

**You need to install Emscripten**

1. Clone the repository.
1. Cd into it.
1. Install submodules. `git submodule update --init --recursive --depth=1`
1. Make a build folder for debug and release. `mkdir build/debug, build/release`
1. Cd into either one.
1. Setup CMake. `emcmake cmake -S ../.. -D CMAKE_BUILD_TYPE=DEBUG`
1. Run the build command. `cmake --build . --parallel`
1. Run a local web server and open the `RaylibTemplate.html`.

### Windows, MinGW

1. Clone the repository.
1. Cd into it.
1. Install submodules. `git submodule update --init --recursive --depth=1`
1. Make a build folder for debug and release. `mkdir build/debug, build/release`
1. Cd into either one.
1. Setup CMake. **Make sure to define MinGW Makefiles if you are using MinGW!** `cmake -DCMAKE_BUILD_TYPE=DEBUG ../.. -G "MinGW Makefiles"`
1. Run the build command. `cmake --build . --parallel`

### Linux

1. Clone the repository.
1. Cd into it.
1. Install submodules. `git submodule update --init --recursive --depth=1`
1. Make a build folder for debug and release. `mkdir -p build/debug build/release`
1. Cd into either one.
1. Setup CMake. `cmake -DCMAKE_BUILD_TYPE=DEBUG ../..`
1. Run the build command. `cmake --build . --parallel`
