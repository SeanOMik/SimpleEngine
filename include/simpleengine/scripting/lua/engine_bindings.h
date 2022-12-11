#pragma once

#include <sol/sol.hpp>

namespace simpleengine::scripting::lua {
    class EngineBindings {
    public:
        static sol::table bind_full_engine(sol::this_state s);

        /// This binds general engine objects, like vec3, mat, quats, etc
        static void bind_math_objects(sol::this_state s);
    };
}