cmake_minimum_required (VERSION 3.6)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
project(SimpleEngine)

include(cmrc/CMakeRC.cmake)
include(CMake/Findsoil2.cmake)

# Add some CMake options:
option(SIMPLE_ENGINE_BUILD_EXAMPLES "Build example projects" ON)

# By default use OpenGL GLVND
option(SIMPLE_ENGINE_USE_GL_LEGACY "Use OpenGL legacy, or use BLVND" OFF)
if (SIMPLE_ENGINE_USE_GL_LEGACY)
    set(OpenGL_GL_PREFERENCE "LEGACY")
else()
    set(OpenGL_GL_PREFERENCE "GLVND")
endif()

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 REQUIRED)
find_package(OpenGL REQUIRED)

# Link sources
file(GLOB_RECURSE source_list src/*.cpp)
add_library(simpleengine STATIC ${source_list})

# Link headers
target_include_directories(simpleengine PUBLIC include PRIVATE include/simpleengine)

# Embed shaders
file(GLOB_RECURSE simpleeng_resources_list resources/**)
cmrc_add_resource_library(
    simpleengine_resources# ALIAS simpleengine::res::shaders
    #NAMESPACE simpleengine__res
    #WHENCE resources/shaders
    #PREFIX resources/shaders
    ${simpleeng_resources_list}
)

# Link dependencies
target_link_libraries(simpleengine PUBLIC GLEW::GLEW)
target_link_libraries(simpleengine PUBLIC glfw)
target_link_libraries(simpleengine PUBLIC glm)
target_link_libraries(simpleengine PUBLIC soil2)
target_link_libraries(simpleengine PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(simpleengine PRIVATE simpleengine_resources)

# Include some dependencies' include directories
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

# Add examples as a target if the user has them enabled
if (SIMPLE_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Set C++ standard to C++20
set_target_properties(simpleengine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)