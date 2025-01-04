cmake_minimum_required(VERSION 3.10)

project(GraphIsomorphism)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # For Windows cross-compilation
    set(FLTK_LIBRARIES 
        fltk
        fltk_forms
        fltk_images
        fltk_gl
    )
    
    # Add Windows-specific libraries
    list(APPEND FLTK_LIBRARIES
        gdi32
        comdlg32
        oleaut32
        ole32
        uuid
        shell32
    )
else()
    # Normal FLTK finding for non-Windows builds
    find_package(FLTK REQUIRED)
    include_directories(${FLTK_INCLUDE_DIR})
endif()

add_executable(GraphIsomorphismChecker main.cpp graph_utils.cpp)
target_link_libraries(GraphIsomorphismChecker ${FLTK_LIBRARIES})

# Ensure .exe extension on Windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(GraphIsomorphismChecker PROPERTIES SUFFIX ".exe")
endif()