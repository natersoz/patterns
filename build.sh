#!/bin/sh
# cmake -DCMAKE_TOOLCHAIN_FILE= -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
cmake --build ./build
