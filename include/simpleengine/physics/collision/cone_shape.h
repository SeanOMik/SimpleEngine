#pragma once

#include <utility>

#include "BulletCollision/CollisionShapes/btConeShape.h"

namespace simpleengine::physics::collision {
    class ConeShape {
        btConeShape inner;
    public:
        ConeShape() = default; 

        ConeShape(btConeShape inner) : inner(std::move(inner)) {

        }

        ConeShape(float radius, float height) : inner(radius, height) {
            
        }

        btConeShape* get_inner() {
            return &inner;
        }
    };
}