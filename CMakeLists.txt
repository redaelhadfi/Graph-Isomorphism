cmake_minimum_required(VERSION 3.10)

project(GraphIsomorphism)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/opt/homebrew/Cellar/fltk/1.4.1/include)

add_executable(GraphIsomorphism main.cpp graph_utils.cpp)

target_link_libraries(GraphIsomorphism
    /opt/homebrew/Cellar/fltk/1.4.1/lib/libfltk.a
    /opt/homebrew/Cellar/fltk/1.4.1/lib/libfltk_forms.a
    /opt/homebrew/Cellar/fltk/1.4.1/lib/libfltk_images.a
    "-framework Cocoa"
    "-framework ScreenCaptureKit"
)
