#pragma once

#include "entt/entity/fwd.hpp"
#include "gfx/mesh.h"
#include "event/event.h"
#include "renderable.h"
#include "simpleengine/gfx/renderer.h"

#include <memory>

#include <GLFW/glfw3.h>
#include <vector>

#include <entt/entt.hpp>

namespace simpleengine {
    namespace ecs {
        class Entity;
    }

    class Scene : public simpleengine::Event {
    protected:
        entt::registry registry;
        std::shared_ptr<gfx::Renderer> renderer;
    public:
        Scene(std::shared_ptr<gfx::Renderer> renderer);

        ecs::Entity create_entity();

        virtual void update(const float& delta_time) override;

        virtual void destroy() override;
    };
}