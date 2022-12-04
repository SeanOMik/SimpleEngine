#pragma once

#include <entt/signal/fwd.hpp>
#include <entt/entt.hpp>

#include <rttr/rttr_enable.h>

namespace simpleengine::ecs {
    class Entity;

    class Registry {
        RTTR_ENABLE()

        entt::registry inner;
        //entt::dispatcher dispatcher;
    public:
        Registry();

        entt::registry& get_inner();
        Entity create_entity();
    };

    /* struct NewEntityEvent {
        Entity& entity;
    }; */
}