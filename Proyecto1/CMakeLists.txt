cmake_minimum_required(VERSION 3.24)
project(Proyecto1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proyecto1 main.cpp Tools.cpp Tools.h Commands.cpp Commands.h Disks.cpp Disks.h Structs.cpp Structs.h Graphviz.cpp Graphviz.h)
