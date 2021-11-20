cmake_minimum_required (VERSION 3.6)
project(DevTesting DESCRIPTION "A testing project for engine developers.")

add_executable(dev_testing src/main.cpp)

# Link headers and source files.
file(GLOB_RECURSE source_list src/*.cpp)
target_sources(dev_testing PRIVATE ${source_list})
target_include_directories(dev_testing PUBLIC include)

# Link simpleengine
target_link_libraries(dev_testing PUBLIC simpleengine)

# Set standard to C++20
set_target_properties(dev_testing PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)