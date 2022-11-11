#pragma once

#include <bullet/BulletCollision/CollisionShapes/btCylinderShape.h>
#include "vector.h"

namespace simpleengine::physics::collision {
    class CylinderShape {
        btCylinderShape inner;
    public:
        CylinderShape(btCylinderShape inner) : inner(inner) {

        }

        /**
         * @brief Create a CylinderShape with the extent of the cylinder shape.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param x_extent The extent of the cylinder shape in the x direction.
         * @param y_extent The extent of the cylinder shape in the y direction.
         * @param z_extent The extent of the cylinder shape in the z direction.
         * 
         */
        CylinderShape(const float& x_extent, const float& y_extent, const float& z_extent) : inner(btVector3(btScalar(x_extent), btScalar(y_extent), btScalar(z_extent))) {

        }

        /**
         * @brief Create a CylinderShape with the extent of the cylinder shape.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param extent The extent of the cylinder.
         * 
         */
        CylinderShape(const simpleengine::Vectorf& extent) : CylinderShape(extent.x(), extent.y(), extent.z()) {

        }

        /**
         * @brief Get the inner bullet cylinder shape object as a pointer.
         * 
         * @return btCylinderShape*
         */
        btCylinderShape* get_inner_ptr() {
            return &inner;
        }

        /**
         * @brief Get the inner bullet cylinder shape object as a reference.
         * 
         * @return btCylinderShape&
         */
        btCylinderShape& get_inner() {
            return inner;
        }
    };
}