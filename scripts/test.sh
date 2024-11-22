#!/bin/bash

cd build
cmake --build . --config Debug
ctest -C Debug
