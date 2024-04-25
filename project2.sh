#!/bin/bash

set -e

if ! dpkg -l cmake &>/dev/null
then
  echo "CMake is not installed. Install using \"sudo apt install cmake\" and"\
        "try again."
  exit 1
else
  echo "Found CMake"
fi

if ! dpkg -l build-essential &>/dev/null
then
  echo "G++ is not installed. Install using \"sudo apt install build-essential\" and"\
        "try again."
  exit 1
else
  echo "Found G++"
fi

if ! dpkg -l libglfw3-dev &>/dev/null
then
  echo "GLFW is not installed. Install using \"sudo apt install libglfw3-dev\" and"\
        "try again."
  exit 1
else
  echo "Found GLFW"
fi

if ! dpkg -l mesa-utils &>/dev/null
then
  echo "Mesa drivers not installed. Install using \"sudo apt install mesa-utils\" and"\
        "try again."
  exit 1
else
  echo "Found Mesa Drivers"
fi

if ! dpkg -l libglu1-mesa-dev &>/dev/null
then
  echo "Mesa OpenGL library not installed. Install using \"sudo apt install libglu1-mesa-dev\" and"\
        "try again."
  exit 1
else
  echo "Found OpenGL Library"
fi

if [ -d build/ ]
then
  rm -r build/ && mkdir build/
else
  mkdir build/
fi

cd build
echo "Building Project 2..."
cmake .. &>/dev/null && make -j$(nproc) &>/dev/null

echo "Running Project 2..."
./project2
