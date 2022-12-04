# Get the sol2 header libraries.
include(FetchContent)
message("Downloading sol2 library...")
set(SOL2_ENABLE_INSTALL OFF)
set(SOL2_BUILD_LUA FALSE)
set(SOL2_LUA_VERSION "5.2")
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)
message("Downloaded sol2 library to: ${sol2_SOURCE_DIR}")
set(sol2_INCLUDE_DIR ${sol2_SOURCE_DIR}/src)