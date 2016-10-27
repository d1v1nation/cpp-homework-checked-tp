cmake_minimum_required(VERSION 3.6)
project(checkedtp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp checked_type.cpp checked_type.h)
add_executable(checkedtp ${SOURCE_FILES})