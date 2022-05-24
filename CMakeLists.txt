cmake_minimum_required(VERSION 3.12)
project(Hmm)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
link_directories(library)
add_executable(Test Hmm.h HmmState.h Hmm1.h Hmm2.h Test/Hmm1Test.cpp Test/Hmm2Test.cpp)
target_link_libraries(Test Math)