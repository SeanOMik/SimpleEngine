cmake_minimum_required (VERSION 3.6)
project(ParticlesExample)

add_executable(particles src/main.cpp)

# Link headers and source files.
file(GLOB_RECURSE source_list src/*.cpp)
target_sources(particles PRIVATE ${source_list})
target_include_directories(particles PUBLIC include)

# Link simpleengine
target_link_libraries(particles PUBLIC simpleengine)

# Set standard to C++17
set_target_properties(particles PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)