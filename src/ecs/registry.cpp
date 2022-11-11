#include "ecs/registry.h"
#include "ecs/entity.h"

namespace simpleengine::ecs {
    Registry::Registry() {

    }

    entt::registry& Registry::get_inner() {
        return inner;
    }
    
    Entity Registry::create_entity() {
        return ecs::Entity(inner, inner.create());
    }
}