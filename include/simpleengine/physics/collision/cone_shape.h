#pragma once

#include <utility>

#include <BulletCollision/CollisionShapes/btConeShape.h>

namespace simpleengine::physics::collision {
    class ConeShape {
        btConeShape inner;
    public:
        ConeShape(btConeShape inner) : inner(std::move(inner)) {

        }

        ConeShape(float radius, float height) : inner(radius, height) {
            
        }

        /**
         * @brief Get the inner bullet cone shape object as a pointer.
         * 
         * @return btConeShape* 
         */
        btConeShape* get_inner_ptr() {
            return &inner;
        }

        /**
         * @brief Get the inner bullet cone shape object as a reference.
         * 
         * @return btConeShape&
         */
        btConeShape& get_inner() {
            return inner;
        }
    };
}