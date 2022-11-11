#pragma once

#include "../../physics/collision/sphere_shape.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a Sphere collision shape.
     * 
     */
    class SphereColliderComponent {
    public:
        physics::collision::SphereShape sphere_shape;

        SphereColliderComponent(physics::collision::SphereShape sphere) : sphere_shape(sphere) {

        }

        SphereColliderComponent(const float& radius) : sphere_shape(radius) {
            
        }
    };
}