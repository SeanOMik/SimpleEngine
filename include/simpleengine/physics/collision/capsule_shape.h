#pragma once

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace simpleengine::physics::collision {
    class CapsuleShape {
        btCapsuleShape inner;
    public:
        CapsuleShape(btCapsuleShape inner) : inner(inner) {

        }

        CapsuleShape(const float& radius, const float& height) : inner(radius, height) {

        }

        /**
         * @brief Get the inner bullet capsule shape object as a pointer.
         * 
         * @return btCapsuleShape* 
         */
        btCapsuleShape* get_inner_ptr() {
            return &inner;
        }

        /**
         * @brief Get the inner bullet capsule shape object as a reference.
         * 
         * @return btCapsuleShape& 
         */
        btCapsuleShape& get_inner() {
            return inner;
        }
    };
}