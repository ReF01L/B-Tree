cmake_minimum_required(VERSION 3.19)
project(B_Tree)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/B-Tree)
add_executable(B_Tree main.cpp src/tree.cpp include/B-Tree/tree.h src/node.cpp include/B-Tree/node.h src/stasus.cpp)