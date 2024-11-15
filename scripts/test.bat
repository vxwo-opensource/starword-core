@echo off

cmake --build . --config Debug
ctest -C Debug
