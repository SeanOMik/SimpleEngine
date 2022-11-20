#pragma once

#include "../../physics/collision/cone_shape.h"
#include "../../vector.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a cone collision shape.
     * 
     */
    class ConeColliderComponent {
    public:
        physics::collision::ConeShape cone_shape;

        ConeColliderComponent(physics::collision::ConeShape cone) : cone_shape(cone) {

        }

        ConeColliderComponent(float radius, float height) : cone_shape(radius, height) {
            
        }
    };
}