cmake_minimum_required(VERSION 3.5)
project(Sat_mac)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Satelite.h User.h Sat.h User.cpp Sat.cpp Satelite.cpp Area2Sat.h Area2Sat.cpp)
add_executable(Sat_mac ${SOURCE_FILES})