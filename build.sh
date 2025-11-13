#!/bin/bash

if [[ ! -d output ]]; then
    echo "Making output directory."
    mkdir output
fi

g++ -g -O3 -fPIC -std=c++17 -Wall -Wextra -Werror --shared khelper.cpp -o output/libkhelper.so \
    && cp khelper.hpp output/

if [[ $1 != "-l" || $1 != "--lib" ]]; then
    g++ -g -O3 -std=c++17 test.cpp -Loutput -lkhelper -o exe_test \
        && LD_LIBRARY_PATH=output ./exe_test
fi

