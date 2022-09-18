# Get the stb header libraries.
include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG 8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55
)
FetchContent_MakeAvailable(stb)
message("Downloaded stb library to: ${stb_SOURCE_DIR}")
set(STB_INCLUDE_DIR ${stb_SOURCE_DIR})