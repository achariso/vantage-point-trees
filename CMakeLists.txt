cmake_minimum_required(VERSION 3.15)
project(1___PThreads___Vantage_Point_Trees C)

set(CMAKE_C_STANDARD 11)

add_executable(VPT main.c src/vpt_parallel.c include/vpt_parallel.h)

include_directories(include)
include_directories(src)

add_subdirectory(src)
add_subdirectory(test)