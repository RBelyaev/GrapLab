cmake_minimum_required(VERSION 3.10)

project(GraphPaint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(GraphPaint Vector2d.h BMP.h BMP.cpp Graph.h Graph.cpp main.cpp)
