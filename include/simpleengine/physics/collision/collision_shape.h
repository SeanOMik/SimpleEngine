#pragma once

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>

namespace simpleengine::physics::collision {
    class CollisionShape {
        btCollisionShape* inner;
    public:
        CollisionShape() = default;

        CollisionShape(btCollisionShape* inner) : inner(inner) {

        }

        btCollisionShape* get_inner() {
            return inner;
        }
    };
}