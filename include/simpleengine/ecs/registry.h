#pragma once

#include "entt/signal/fwd.hpp"
#include <entt/entt.hpp>

namespace simpleengine::ecs {
    class Entity;

    class Registry {
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