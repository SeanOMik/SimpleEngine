# Get the entt header libraries.
include(FetchContent)
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.10.3
)
FetchContent_MakeAvailable(entt)
message("Downloaded entt library to: ${entt_SOURCE_DIR}")
set(ENTT_INCLUDE_DIR ${entt_SOURCE_DIR}/src)