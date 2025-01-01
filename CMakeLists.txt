cmake_minimum_required(VERSION 3.29)
project(memory_alloc C)

set(CMAKE_C_STANDARD 11)

add_executable(memory_alloc main.c memalloc.c)