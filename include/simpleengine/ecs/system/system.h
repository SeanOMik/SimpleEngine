#pragma once

#include "../../renderable.h"

#include <rttr/rttr_enable.h>

namespace simpleengine::ecs {
    class Entity;
    class Registry;

    namespace system {
        class System : public simpleengine::Renderable {
            RTTR_ENABLE()
        protected:
            std::shared_ptr<Registry> entity_registry;
        public:
            System(std::shared_ptr<Registry> entity_registry);

            ecs::Entity create_entity();

            virtual void update(const float& delta_time) = 0;
            virtual void input_update(const float& delta_time) = 0;
            virtual void render(const float& interpolate_alpha, const float& frame_time) = 0;
            virtual void destroy() = 0;
        };
    }
}