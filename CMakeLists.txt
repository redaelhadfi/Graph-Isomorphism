cmake_minimum_required(VERSION 3.10)

project(GraphIsomorphism)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIR})

add_executable(GraphIsomorphism main.cpp graph_utils.cpp)

target_link_libraries(GraphIsomorphism ${FLTK_LIBRARIES})

cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -G "Ninja"
