#include <cstdint>

#ifdef _WIN32
#include "windows.h"
#elif __linux__
#include <unistd.h>
#endif

namespace simpleengine {
    namespace util {
#ifdef _WIN32
        inline uint32_t get_current_pid(LPCTSTR ProcessName) // non-conflicting function name
        {
            return GetCurrentProcessId();
        }
#elif __linux__
        inline uint32_t get_current_pid() {
            return ::getpid();
        }
#endif
    }
}