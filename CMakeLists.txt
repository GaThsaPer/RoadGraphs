cmake_minimum_required(VERSION 3.16)
project(project3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

set(GRAPHICS "GRAPHICS_API_OPENGL_33" CACHE STRING "" FORCE)
set(PLATFORM "Desktop" CACHE STRING "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)


#Add additional content directory
add_subdirectory(external/libxl)

#Main project (Source Code)
include_directories(include)
set(SOURCES
    src/main.cpp
    src/DownloadData.cpp
    src/Node.cpp
    src/Roads.cpp
    src/Input.cpp
    src/visualization.cpp
    src/Djikstra.cpp
    )

#Executable file add
add_executable(project3 ${SOURCES})

#Import Library
target_include_directories(project3 PRIVATE include external/libxl/include_cpp)
target_link_libraries(project3 PRIVATE libxl raylib)