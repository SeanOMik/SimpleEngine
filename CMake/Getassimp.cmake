include(ExternalProject)
ExternalProject_Add(external_assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.2.5
    GIT_SUBMODULES_RECURSE true
    GIT_PROGRESS true
    CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
    -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -D ASSIMP_INSTALL=NO
    -D ASSIMP_BUILD_TESTS=NO
)
message("Downloaded assimp library")