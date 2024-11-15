#!/bin/bash

cmake --build . --config Debug
ctest -C Debug

