#!/usr/bin/env bash

cd grammar
bison -d --output Parser.cpp grammar.ypp
cd ..

mkdir -p build
cd build

cp ../test.txt .

cmake .. || exit 1
make || exit 1
