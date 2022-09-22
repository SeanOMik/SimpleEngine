set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)

# Get the tinygltf header libraries.
include(FetchContent)
FetchContent_Declare(
    tinygltf
    GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
    GIT_TAG eec4c98862b7fb760b2fb70971d7b652e593af9f
)
FetchContent_MakeAvailable(tinygltf)

message("Downloaded tinygltf library to: ${tinygltf_SOURCE_DIR}")

#set(STB_INCLUDE_DIR ${stb_SOURCE_DIR})