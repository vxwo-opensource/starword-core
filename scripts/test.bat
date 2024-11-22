@echo off
setlocal

cd build
cmake --build . --config Debug
ctest -C Debug
