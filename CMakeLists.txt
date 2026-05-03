cmake_minimum_required(VERSION 3.22)

project(nativeime)

add_library(keyboard SHARED keyboard.cpp)

find_library(keyboard log)

target_link_libraries(keyboard ${log-lib})