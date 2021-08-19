cmake_minimum_required(VERSION 3.16)
project(B_Tree)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/B-Tree)
add_executable(B_Tree main.cpp include/B-Tree/tree.h include/B-Tree/node.h tests/tree.cpp tests/node.cpp)