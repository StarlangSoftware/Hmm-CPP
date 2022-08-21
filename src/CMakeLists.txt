cmake_minimum_required(VERSION 3.12)
project(Hmm)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(Test src/Hmm.h src/HmmState.h src/Hmm1.h src/Hmm2.h Test/Hmm1Test.cpp Test/Hmm2Test.cpp)
target_link_libraries(Test ${CONAN_LIBS_MATH})