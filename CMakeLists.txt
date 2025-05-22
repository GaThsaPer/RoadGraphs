cmake_minimum_required(VERSION 3.16)
project(project3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Add additional content directory
add_subdirectory(external/libxl)

#Main project (Source Code)
include_directories(include)
set(SOURCES
    src/main.cpp
    src/DownloadData.cpp
    src/Node.cpp
    )

#Executable file add
add_executable(project3 ${SOURCES})

#Import Library
target_include_directories(project3 PRIVATE include external/libxl/include_cpp)
target_link_libraries(project3 PRIVATE libxl)