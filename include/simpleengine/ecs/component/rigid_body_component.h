#pragma once

#include "../../gfx/model.h"
#include "simpleengine/vector.h"

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btScalar.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include <btBulletDynamicsCommon.h>

namespace simpleengine::physics {
    class PhysicsSystem;
}

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a Model that will be rendered.
     * 
     */
    class RigidBodyComponent {
    friend simpleengine::physics::PhysicsSystem;
    private:
        float mass;
        bool is_dynamic;

        // TODO: Free
        btDefaultMotionState* motion_state;
        btCollisionShape* col_shape;
    public:
        //bool initialized = false;
        btRigidBody* rigid_body;

        RigidBodyComponent(float mass, simpleengine::Vectorf start_origin) : mass(mass), is_dynamic(mass != 0.f), rigid_body(nullptr) {

        }
    };
}