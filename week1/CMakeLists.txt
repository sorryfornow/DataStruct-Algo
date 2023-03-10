cmake_minimum_required(VERSION 3.22)
project(week1 C)

set(CMAKE_C_STANDARD 11)

add_executable(week1 binary.c IntStack.h IntStack.c)
