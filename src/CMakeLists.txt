cmake_minimum_required(VERSION 3.13)
project(1___PThreads___Vantage_Point_Trees C)

set(CMAKE_C_STANDARD 99)

set(VPT_SOURCES vptree.c utils.c)
add_library(VPT_LIB ${VPT_SOURCES})

target_link_libraries(VPT VPT_LIB pthread sodium m)