#pragma once

#include "../../physics/collision/capsule_shape.h"
#include "../../vector.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a capsule collision shape.
     * 
     */
    class CapsuleColliderComponent {
    public:
        physics::collision::CapsuleShape capsule_shape;

        CapsuleColliderComponent(physics::collision::CapsuleShape capsule) : capsule_shape(capsule) {

        }

        CapsuleColliderComponent(const float& radius, const float& height) : capsule_shape(radius, height) {

        }
    };
}