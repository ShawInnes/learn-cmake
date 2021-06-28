#!/bin/sh

export RPI_ROOT=/pitools
export RPI_LIBS=${RPI_ROOT}/libs
export PATH=$PATH:${RPI_ROOT}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin

# Set up environment variables for cross-compiling those dependencies not using CMake.
export CROSS=arm-linux-gnueabihf
export CC=${CROSS}-gcc
export LD=${CROSS}-ld
export AS=${CROSS}-as
export AR=${CROSS}-ar
