#pragma once

#include "system.h"

#include <rttr/rttr_enable.h>

namespace simpleengine {
    // fwd decl
    class Camera;
    namespace gfx {
        class Renderer;
    }

    namespace ecs {
        class Entity;
        class Registry;

        namespace system {
            class SceneSystem : public System {
                RTTR_ENABLE()
            protected:
                std::shared_ptr<gfx::Renderer> renderer;
                std::shared_ptr<Camera> camera;
            public:
                SceneSystem(std::shared_ptr<Registry> entity_registry, std::shared_ptr<gfx::Renderer> renderer, std::shared_ptr<Camera> camera);

                ecs::Entity create_entity();

                virtual void update(const float& delta_time) override;
                virtual void input_update(const float& delta_time) override;
                virtual void render(const float& interpolate_alpha, const float& frame_time) override;
                virtual void destroy() override;
            };
        }
    }
}