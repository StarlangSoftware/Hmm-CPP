cmake_minimum_required(VERSION 3.12)
project(Hmm)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

find_package(math REQUIRED)
find_package(data_structure REQUIRED)

add_executable(Test src/Hmm.h src/HmmState.h src/Hmm1.h src/Hmm2.h Test/Hmm1Test.cpp Test/Hmm2Test.cpp)
target_link_libraries(Test math::math data_structure::data_structure)