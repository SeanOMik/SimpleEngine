cmake_minimum_required (VERSION 3.6)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
project(SimpleEngine)

include(cmrc/CMakeRC.cmake)
include(CMake/GetStbLibraries.cmake)
include(CMake/GetEnTT.cmake)

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
find_package(OpenGL REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Link sources
file(GLOB_RECURSE source_list src/*.cpp)
add_library(simpleengine STATIC ${source_list})

target_compile_definitions(simpleengine PUBLIC SPDLOG_FMT_EXTERNAL)

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
target_link_libraries(simpleengine PUBLIC ${GLM_LIBRARIES})
target_link_libraries(simpleengine PUBLIC ${OPENGL_LIBRARIES})
if(WIN32)
    target_link_libraries(simpleengine PUBLIC assimp::assimp)
    target_link_libraries(simpleengine PUBLIC fmt::fmt)
    target_link_libraries(simpleengine PUBLIC spdlog::spdlog)
else()
    target_link_libraries(simpleengine PUBLIC assimp)
    target_link_libraries(simpleengine PUBLIC fmt)
    target_link_libraries(simpleengine PUBLIC spdlog)
endif()

# Link resources
target_link_libraries(simpleengine PUBLIC cmrc::base)
target_link_libraries(simpleengine PRIVATE simpleengine_resources)

# Include some dependencies' include directories
target_include_directories(simpleengine PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(simpleengine PUBLIC ${GLM_INCLUDE_DIRS})
target_include_directories(simpleengine PUBLIC ${STB_INCLUDE_DIR})
target_include_directories(simpleengine PUBLIC ${ENTT_INCLUDE_DIR})

# Add examples as a target if the user has them enabled
if (SIMPLE_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Set C++ standard to C++20
set_target_properties(simpleengine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)