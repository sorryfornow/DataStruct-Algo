cmake_minimum_required(VERSION 3.23)
project(RBTree C)

set(CMAKE_C_STANDARD 99)

add_executable(RBTree main.c RBTree.h RBTree.c)
