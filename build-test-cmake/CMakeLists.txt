cmake_minimum_required(VERSION 3.16)

project(CarouselReverbTest CXX)

message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build system: ${CMAKE_BUILD_SYSTEM}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")

add_executable(test_app src/entry.cpp)
