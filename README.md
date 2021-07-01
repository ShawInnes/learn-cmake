# learn-cmake


docker build . -t ubuntu-build-agent
docker run -it -v ~/devops/learn-cmake:/src ubuntu-build-agent

### Compile on Linux/OSX
```bash
cd /build
cmake /src
make
```

### Compile on Windows
```bash
"C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 16" -DBOOST_ROOT=D:\boost ..
msbuild cppdemo.sln
```

### Compile for ARM
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=/src/cross-compile/Toolchain-rpi.cmake /src
```

## TODO

### Install Boost - Windows
```bash
wget https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/boost_1_75_0.tar.gz
tar zxvf boost_1_75_0.tar.gz
  
cd boost_1_75_0
./bootstrap.bat
./b2 --with-thread --with-chrono --with-system --prefix=d:/boost link=shared variant=release threading=multi install
```

### Install Boost - Cross Compile
```bash
export RPI_ROOT=/pitools
export RPI_LIBS=${RPI_ROOT}/libs
export PATH=$PATH:${RPI_ROOT}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
export SYSROOT=${RPI_ROOT}/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot

# Set up environment variables for cross-compiling those dependencies not using CMake.
export CROSS=arm-linux-gnueabihf
export CC=${CROSS}-gcc
export LD=${CROSS}-ld
export AS=${CROSS}-as
export AR=${CROSS}-ar

echo "import option ; import feature ; using gcc : arm : arm-linux-gnueabihf-g++ ;" > ~/user-config.jam

mkdir -p $RPI_ROOT/deps
cd $RPI_ROOT/deps

wget https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/boost_1_75_0.tar.gz
tar zxvf boost_1_75_0.tar.gz
  
cd boost_1_75_0
./bootstrap.sh
./b2 install toolset=gcc-arm --prefix=${SYSROOT} --with-thread --with-chrono --with-system --stagedir=. stage
```
