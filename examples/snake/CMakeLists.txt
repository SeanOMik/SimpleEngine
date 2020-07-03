cmake_minimum_required (VERSION 3.6)
project(Snake)

add_executable(snake src/main.cpp)

# Link headers and source files.
file(GLOB_RECURSE source_list src/*.cpp)
target_sources(snake PRIVATE ${source_list})
target_include_directories(snake PUBLIC include)

# Link simpleengine
target_link_libraries(snake PUBLIC simpleengine)

# Set standard to C++17
set_target_properties(snake PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)