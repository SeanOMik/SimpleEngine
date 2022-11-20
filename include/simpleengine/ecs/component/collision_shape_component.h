#pragma once

#include "../../physics/collision/collision_shape.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a collision shape.
     * 
     */
    class CollisionShapeComponent {
    public:
        physics::collision::CollisionShape* collision_shape;

        CollisionShapeComponent(physics::collision::CollisionShape* shape) : collision_shape(shape) {

        }
    };
}