cmake_minimum_required(VERSION 3.15)
project(1___PThreads___Vantage_Point_Trees C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(VPT ${SOURCE_FILES})

include_directories(inc)
include_directories(src)

add_subdirectory(src)
add_subdirectory(test)