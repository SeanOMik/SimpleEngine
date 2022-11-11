#pragma once

#include "../../physics/collision/box_shape.h"

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a Box collision shape.
     * 
     */
    class BoxColliderComponent {
    public:
        physics::collision::BoxShape box_shape;

        BoxColliderComponent(physics::collision::BoxShape box) : box_shape(box) {

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
        BoxColliderComponent(const float& x_extent, const float& y_extent, const float& z_extent) : box_shape(x_extent, y_extent, z_extent) {

        }

        /**
         * @brief Create a BoxShape with the extent of the box.
         *
         * @note These extents are in half-extents since that's what the underlying Physics engine uses (Bullet).
         *
         * @param extent The extent of the box.
         * 
         */
        BoxColliderComponent(const simpleengine::Vectorf& extent) : box_shape(extent) {

        }
    };
}