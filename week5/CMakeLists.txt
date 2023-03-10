cmake_minimum_required(VERSION 3.22)
project(week5 C)

set(CMAKE_C_STANDARD 11)

add_executable(week5 main.c WGraph.c WGraph.h PQueue.c)
