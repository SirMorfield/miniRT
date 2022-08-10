#!/bin/bash

echo ""
echo "c -O0"
gcc -O0 normalize.c
./a.out
echo ""
echo "cpp -O0"
clang++ -O0 normalize.cpp
./a.out

echo ""
echo "c -O3"
gcc -O3 normalize.c
./a.out
echo ""
echo "cpp -O3"
clang++ -O3 normalize.cpp
./a.out
