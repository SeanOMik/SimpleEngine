#pragma once

#include <memory>

namespace simpleengine::ecs {
    class Registry;
}

namespace simpleengine::scripting {
    class ScriptingEngine {
    protected:
        std::shared_ptr<ecs::Registry> entity_registry;
    public:
        ScriptingEngine(std::shared_ptr<ecs::Registry> entity_registry) : entity_registry(entity_registry) {

        }

        /// Setup the scripting language VM
        virtual void setup_language() = 0;

        /// Expose simpleengine to the scripting language
        virtual void expose_simpleengine() = 0;

        virtual void run_script(std::string lua_code) = 0;
        virtual void run_script_file(std::string path) = 0;

        virtual void update(const float& delta_time) = 0;
        virtual void input_update(const float& delta_time) = 0;
        virtual void render(const float& interpolate_alpha, const float& frame_time) = 0;
        virtual void destroy() = 0;
    };
}