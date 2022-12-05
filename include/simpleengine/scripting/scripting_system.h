#pragma once

#include "../ecs/system/system.h"
#include "scripting_engine.h"
#include <memory>

namespace simpleengine::scripting {
    class ScriptingSystem : public simpleengine::ecs::system::System {
    private:
        std::unordered_map<const char*, std::unique_ptr<simpleengine::scripting::ScriptingEngine>> scripting_engines;
    public:
        ScriptingSystem(std::shared_ptr<simpleengine::ecs::Registry> entity_registry);

        void add_lua_engine();

        virtual void update(const float& delta_time) override;
        virtual void input_update(const float& delta_time) override;
        virtual void render(const float& interpolate_alpha, const float& frame_time) override;
        virtual void destroy() override;
    };
}