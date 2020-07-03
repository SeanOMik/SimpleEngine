cmake_minimum_required (VERSION 3.6)
project(first_example)

add_executable(first_example src/main.cpp)

# Link headers and source files.
file(GLOB_RECURSE source_list src/*.cpp)
target_sources(first_example PRIVATE ${source_list})
target_include_directories(first_example PUBLIC include)

# Link simpleengine
target_link_libraries(first_example PUBLIC simpleengine)

# Set standard to C++17
set_target_properties(first_example PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)