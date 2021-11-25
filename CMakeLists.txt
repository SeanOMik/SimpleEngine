cmake_minimum_required (VERSION 3.6)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
project(SimpleEngine)

include(cmrc/CMakeRC.cmake)

# Add some CMake options:
option(SIMPLE_ENGINE_BUILD_EXAMPLES "Build example projects" ON)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)

# Link sources
file(GLOB_RECURSE source_list src/*.cpp)
add_library(simpleengine STATIC ${source_list})

# Link headers
target_include_directories(simpleengine PUBLIC include PRIVATE include/simpleengine)

# Link dependencies
target_link_libraries(simpleengine PUBLIC GLEW::GLEW)
target_link_libraries(simpleengine PUBLIC glfw)
target_link_libraries(simpleengine PUBLIC glm::glm)
target_link_libraries(simpleengine PUBLIC soil2)

include_directories(${GLM_INCLUDE_DIRS})

# Add examples as a target if the user has them enabled
if (SIMPLE_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Set C++ standard to C++20
set_target_properties(simpleengine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)