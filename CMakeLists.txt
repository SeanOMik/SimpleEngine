cmake_minimum_required (VERSION 3.6)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
project(GameEngine)

# Add some CMake options:
option(BUILD_EXAMPLES "Build example projects" ON)

set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_SYSTEM ON)

find_package(SFML REQUIRED COMPONENTS system main window graphics audio)

# Link sources
file(GLOB_RECURSE source_list src/*.cpp)
add_library(gameengine STATIC ${source_list})

# Link headers
target_include_directories(gameengine PUBLIC include PRIVATE include/gameengine)

# Link dependencies
target_link_libraries(gameengine PUBLIC sfml-system sfml-main sfml-window OpenGL sfml-network sfml-graphics Freetype OpenAL Vorbis FLAC sfml-audio)

# Add examples as a target if the user has them enabled
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Set C++ standard to C++17
set_target_properties(gameengine PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)