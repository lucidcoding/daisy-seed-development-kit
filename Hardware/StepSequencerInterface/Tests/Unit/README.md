1. Download from https://github.com/google/googletest/tree/main 


Following: https://google.github.io/googletest/quickstart-cmake.html
do `gcc -v` and `g++ -v` to find locations and set up:
SET CMAKE_C_COMPILER=C:\mingw64\bin\gcc.exe
SET CMAKE_CXX_COMPILER=C:\mingw64\bin\g++.exe
set( CMAKE_MAKE_PROGRAM "C:/Program Files/DaisyToolchain-0.3.1/bin/make.exe" )

run with `cmake -S . -B build -G "MinGW Makefiles"`
cmake --build build -G "MinGW Makefiles"

