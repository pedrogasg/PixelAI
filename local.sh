#/bin/sh

mkdir -p build
cd build
cmake -S ../ -B .
make && make Shaders && ./PaiEngine
cd ..