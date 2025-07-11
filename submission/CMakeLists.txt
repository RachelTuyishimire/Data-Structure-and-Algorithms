cmake_minimum_required(VERSION 3.1)
project( assignment2)
find_package(PkgConfig REQUIRED)

file(GLOB_RECURSE assignment0_src
    "src/*.cpp"
    "src/*/*.cpp"
)

add_executable(assignment2  ${assignment0_src})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
target_link_libraries(assignment2)
#set(EXECUTABLE_OUTPUT_PATH "../bin")
set(CMAKE_BINARY_DIR "../bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
