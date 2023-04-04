#!/bin/bash

git submodule update --init --recursive

# 编译PcapPlusPlus
cd PcapPlusPlus
cmake -S . -B build
cmake --build build
DESTDIR=../sniffer/third_party/PcapPlusPlus cmake --install build --prefix "/"
cd -

# 编译SQLiteCpp
cd SQLiteCpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DSQLITECPP_USE_ASAN=OFF -DSQLITECPP_USE_GCOV=OFF -DSQLITECPP_BUILD_EXAMPLES=ON -DSQLITECPP_BUILD_TESTS=ON ..
cmake --build build
# ctest --output-on-failure
DESTDIR=../sniffer/third_party/SQLiteCpp cmake --install build --prefix "/"
cd -

# 编译Boost.asio
wget -O boost.tar.gz https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz
tar -xvf boost.tar.gz
cd boost_1_81_0
./bootstrap.sh
./b2 --prefix=$PWD/../sniffer/third_party/boost --with-system install
cd -

# 编译sniffer
cd sniffer
cmake -S . -B build
cmake --build build
cp build/Sniffer ../Sniffer