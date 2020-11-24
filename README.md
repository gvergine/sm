# sm - state machines for the modern devop

[Build](https://github.com/gvergine/sm/workflows/Build/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/gvergine/sm/badge.svg?branch=master&kill_cache=1)](https://coveralls.io/github/gvergine/sm)

The sm project consist of
- libsm - a pure C state machine library
- smi - a state machine parser and runner for shell use-cases.

## Build from source

```
sudo apt install bison flex gengetopt
git clone https://github.com/gvergine/sm.git
cd sm
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Debug build

The following instructions are useful only to who want to build a debug version
of this project (and hopefully contribute to the project):

1. Install build and test tools and clone the project:
```
sudo apt install bison flex gengetopt check subunit lcov valgrind 
git clone https://github.com/gvergine/sm.git
cd sm
```

2. Create a build directory and invoke cmake
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
env CTEST_OUTPUT_ON_FAILURE=1 ctest -R sm-unit-tests -T memcheck
env CTEST_OUTPUT_ON_FAILURE=1 ctest -R smi-tests # Don't use -T memcheck here!
make sm_coverage smi_coverage
```
