#!/usr/bin/env bash
echo $1
./build.sh || exit 1

cp $1 ./build

cd build

./ICompiler $1
