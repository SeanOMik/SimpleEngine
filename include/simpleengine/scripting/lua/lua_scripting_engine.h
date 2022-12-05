#pragma once

#include "../scripting_engine.h"
#include <string>

#include <sol/sol.hpp>

namespace simpleengine::ecs {
    class Registry;
}

namespace simpleengine::scripting::lua {
    class LuaScriptingEngine : public ScriptingEngine {
    private:
        sol::state lua;
        std::vector<sol::protected_function_result> script_results;
    public:
        LuaScriptingEngine(std::shared_ptr<ecs::Registry> entity_registry);

        static void lua_panic_handler(sol::optional<std::string> maybe_msg);
        static int lua_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description);
        static sol::protected_function_result lua_protected_function_handler(lua_State* lstate, sol::protected_function_result result);

        virtual void setup_language() override;
        virtual void expose_simpleengine() override;

        // TODO: Add some way for the user to get a reference to a script so they can stop it from running
        virtual void run_script(std::string lua_code) override;
        virtual void run_script_file(std::string path) override;

        virtual void update(const float& delta_time) override;
        virtual void input_update(const float& delta_time) override;
        virtual void render(const float& interpolate_alpha, const float& frame_time) override;
        virtual void destroy() override;
    };
}