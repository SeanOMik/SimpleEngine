#pragma once

#include "../../physics/collision/cylinder_shape.h"
#include "../../vector.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a cylinder collision shape.
     * 
     */
    class CylinderColliderComponent {
    public:
        physics::collision::CylinderShape cylinder_shape;

        CylinderColliderComponent(physics::collision::CylinderShape cylinder) : cylinder_shape(cylinder) {

        }

        /**
         * @brief Create a cylinder collider with the extent of the cylinder shape.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param x_extent The extent of the cylinder shape in the x direction.
         * @param y_extent The extent of the cylinder shape in the y direction.
         * @param z_extent The extent of the cylinder shape in the z direction.
         * 
         */
        CylinderColliderComponent(const float& x_extent, const float& y_extent, const float& z_extent) : cylinder_shape(x_extent, y_extent, z_extent) {

        }

        /**
         * @brief Create a CylinderShape with the extent of the cylinder shape.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param extent The extent of the cylinder.
         * 
         */
        CylinderColliderComponent(const simpleengine::Vectorf& extent) : cylinder_shape(extent) {

        }
    };
}