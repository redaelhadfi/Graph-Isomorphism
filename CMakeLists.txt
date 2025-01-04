cmake_minimum_required(VERSION 3.10)
project(GraphIsomorphism)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect Windows platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Include FLTK directories for MinGW
    include_directories(/usr/x86_64-w64-mingw32/include)
    link_directories(/usr/x86_64-w64-mingw32/lib)
    
    # Specify FLTK libraries
    set(FLTK_LIBRARIES
        fltk
        fltk_forms
        fltk_images
        fltk_gl
    )

    # Additional system libraries for Windows
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

# Add executable target
add_executable(GraphIsomorphismChecker main.cpp)

# Link FLTK libraries
target_link_libraries(GraphIsomorphismChecker ${FLTK_LIBRARIES})

# Set static linking on Windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(GraphIsomorphismChecker PROPERTIES
        LINK_FLAGS "-static -static-libgcc -static-libstdc++"
    )
endif()
