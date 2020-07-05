cmake_minimum_required (VERSION 3.6)
project(AnimationExample)

add_executable(animation_example src/main.cpp)

# Link headers and source files.
file(GLOB_RECURSE source_list src/*.cpp)
target_sources(animation_example PRIVATE ${source_list})
target_include_directories(animation_example PUBLIC include)

# Link simpleengine
target_link_libraries(animation_example PUBLIC simpleengine)

# Set standard to C++17
set_target_properties(animation_example PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)