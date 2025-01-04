cmake_minimum_required(VERSION 3.10)
project(GraphIsomorphism)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # FLTK libraries for Windows
    set(FLTK_LIBRARIES
        fltk
        fltk_forms
        fltk_images
        fltk_gl
    )
    
    # Windows system libraries needed by FLTK
    list(APPEND FLTK_LIBRARIES
        gdi32
        ole32
        uuid
        comctl32
        comdlg32
        wsock32
    )
else()
    find_package(FLTK REQUIRED)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include_directories(/usr/x86_64-w64-mingw32/include)
endif()

# Add executable
add_executable(GraphIsomorphismChecker main.cpp)

# Link libraries
target_link_libraries(GraphIsomorphismChecker ${FLTK_LIBRARIES})

# Ensure static linking for Windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(GraphIsomorphismChecker PROPERTIES
        LINK_FLAGS "-static -static-libgcc -static-libstdc++"
    )
endif()