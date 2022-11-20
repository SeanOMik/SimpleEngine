#pragma once

#include "../../vector.h"
#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace simpleengine::physics::collision {
    class BoxShape {
        btBoxShape inner;
    public:
        BoxShape(btBoxShape inner) : inner(inner) {

        }

        /**
         * @brief Create a BoxShape with the extent of the box.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param x_extent The extent of the box in the x direction.
         * @param y_extent The extent of the box in the y direction.
         * @param z_extent The extent of the box in the z direction.
         * 
         */
        BoxShape(const float& x_extent, const float& y_extent, const float& z_extent) : inner(btVector3(btScalar(x_extent), btScalar(y_extent), btScalar(z_extent))) {

        }

        /**
         * @brief Create a BoxShape with the extent of the box.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param extent The extent of the box.
         * 
         */
        BoxShape(const simpleengine::Vectorf& extent) : BoxShape(extent.x(), extent.y(), extent.z()) {

        }

        /**
         * @brief Get the inner bullet box shape object as a pointer.
         * 
         * @return btBoxShape* 
         */
        btBoxShape* get_inner_ptr() {
            return &inner;
        }

        /**
         * @brief Get the inner bullet box shape object as a reference.
         * 
         * @return btBoxShape& 
         */
        btBoxShape& get_inner() {
            return inner;
        }
    };
}