cmake_minimum_required (VERSION 3.6)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
project(SimpleEngine)

# Add some CMake options:
option(BUILD_EXAMPLES "Build example projects" ON)

set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_SYSTEM ON)

find_package(SFML REQUIRED COMPONENTS system main window graphics audio)

# Link sources
file(GLOB_RECURSE source_list src/*.cpp)
add_library(simpleengine STATIC ${source_list})

# Link headers
target_include_directories(simpleengine PUBLIC include PRIVATE include/simpleengine)

# Link dependencies
target_link_libraries(simpleengine PUBLIC sfml-system sfml-main sfml-window OpenGL sfml-network sfml-graphics Freetype OpenAL Vorbis FLAC sfml-audio)

# Add examples as a target if the user has them enabled
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Set C++ standard to C++17
set_target_properties(simpleengine PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)