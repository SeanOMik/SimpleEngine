#pragma once

#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace simpleengine::physics::collision {
    class SphereShape {
        btSphereShape inner;
    public:
        SphereShape(btSphereShape inner) : inner(inner) {

        }

        SphereShape(const float& radius) : inner(btScalar(radius)) {
            
        }

        /**
         * @brief Get the inner bullet sphere shape object as a pointer.
         * 
         * @return btSphereShape* 
         */
        btSphereShape* get_inner_ptr() {
            return &inner;
        }

        /**
         * @brief Get the inner bullet sphere shape object as a reference.
         * 
         * @return btSphereShape&
         */
        btSphereShape& get_inner() {
            return inner;
        }
    };
}