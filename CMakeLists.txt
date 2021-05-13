cmake_minimum_required(VERSION 3.16)
project(lab_5)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_5 main.cpp r-tree/r_tree.cpp r-tree/r_tree.h data/place.h data/place_filereader.h r-tree/Rectangle.cpp r-tree/Rectangle.h r-tree/Circle.cpp r-tree/Circle.h)