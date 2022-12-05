#include "scripting/scripting_system.h"
#include "ecs/system/system.h"

#include "scripting/lua/lua_scripting_engine.h"

namespace simpleengine::scripting {
    
    ScriptingSystem::ScriptingSystem(std::shared_ptr<simpleengine::ecs::Registry> entity_registry) : simpleengine::ecs::system::System(entity_registry) {

    }

    void ScriptingSystem::add_lua_engine() {
        auto lua = std::make_unique<lua::LuaScriptingEngine>(entity_registry);
        
        lua->setup_language();
        lua->expose_simpleengine();

        scripting_engines.emplace("lua", std::move(lua));
    }

    void ScriptingSystem::update(const float& delta_time) {
        for (const auto& [key, val] : scripting_engines) {
            val->update(delta_time);
        }
    }

    void ScriptingSystem::input_update(const float& delta_time) {
        for (const auto& [key, val] : scripting_engines) {
            val->input_update(delta_time);
        }
    }

    void ScriptingSystem::render(const float& interpolate_alpha, const float& frame_time) {
        for (const auto& [key, val] : scripting_engines) {
            val->render(interpolate_alpha, frame_time);
        }
    }
    
    void ScriptingSystem::destroy() {
        std::cout << "Scripting system destroyed!" << std::endl;
    }
}