cmake_minimum_required(VERSION 3.12)

project(Hmm)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(library/Math-CPP library/DataStructure-CPP)

include_directories(library/Math-CPP library/DataStructure-CPP)

link_directories(library)

add_executable(TestHmm Hmm.h HmmState.h Hmm1.h Hmm2.h TestHmm.cpp)

target_link_libraries(TestHmm Math)
