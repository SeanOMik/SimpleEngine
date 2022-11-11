#pragma once

#include "../../gfx/model.h"
#include "simpleengine/vector.h"

#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/LinearMath/btDefaultMotionState.h>
#include <bullet/LinearMath/btScalar.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btVector3.h>

#include <bullet/btBulletDynamicsCommon.h>

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