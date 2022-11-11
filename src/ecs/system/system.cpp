#include "ecs/system/system.h"
#include "ecs/entity.h"
#include "ecs/registry.h"

namespace simpleengine::ecs::system {
    System::System(std::shared_ptr<Registry> entity_registry) : entity_registry(entity_registry) {

    }

    ecs::Entity System::create_entity() {
        return entity_registry->create_entity();
    }
}