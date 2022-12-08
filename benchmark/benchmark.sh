#!/bin/bash

echo "cpp -O0"
clang++ -O0 -std=c++17 normalize.cpp
./a.out

echo ""
echo "cpp -O3"
clang++ -O3 -std=c++17 normalize.cpp
./a.out
