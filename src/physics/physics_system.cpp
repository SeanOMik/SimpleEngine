#include "ecs/component/rigid_body_component.h"
#include "ecs/system/system.h"
#include "physics/physics_system.h"
#include "ecs/component/transform_component.h"
#include "ecs/entity.h"
#include "ecs/registry.h"
#include "game.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <memory>

namespace simpleengine::physics {
    PhysicsSystem::PhysicsSystem(std::shared_ptr<ecs::Registry> entity_registry) : simpleengine::ecs::system::System(entity_registry),
            logger(log::LoggerManager::create_logger("physics")), should_simulate(true), gravity_vector(0, -8, 0) {
        collision_configuration = std::make_unique<btDefaultCollisionConfiguration>();
        col_dispatcher = std::make_unique<btCollisionDispatcher>(collision_configuration.get());
        overlapping_pair_cache = std::make_unique<btDbvtBroadphase>();
        solver = std::make_unique<btSequentialImpulseConstraintSolver>();

        dynamics_world = std::make_unique<btDiscreteDynamicsWorld>(col_dispatcher.get(), overlapping_pair_cache.get(), solver.get(), collision_configuration.get());
        dynamics_world->setGravity(btVector3(gravity_vector.x(), gravity_vector.y(), gravity_vector.z()));
    }

    PhysicsSystem::~PhysicsSystem() {
        
    }

    void PhysicsSystem::set_y_gravity(const float& gravity) {
        this->gravity_vector.set_y(gravity);
    }

    void PhysicsSystem::add_rigid_body(btRigidBody* rigid_body) {
        dynamics_world->addRigidBody(rigid_body);
    }

    void PhysicsSystem::update(const float& delta_time) {
        if (should_simulate) {
            dynamics_world->stepSimulation(delta_time, 10);

            entity_registry->get_inner().view<simpleengine::ecs::TransformComponent, simpleengine::ecs::RigidBodyComponent>().each([this](simpleengine::ecs::TransformComponent& transform_comp, simpleengine::ecs::RigidBodyComponent& rigid_body_comp) {
                
                //transform_comp
                if (!rigid_body_comp.initialized) {
                    btTransform start_transform;
                    start_transform.setIdentity();

                    glm::vec3 origin = transform_comp.get_pos();
                    start_transform.setOrigin(btVector3(origin.x, origin.y, origin.z));

                    glm::quat rot = transform_comp.get_rotation_quat();
                    start_transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

                    btScalar mass_scalar(rigid_body_comp.mass);

                    btVector3 local_inertia;
                    if (rigid_body_comp.is_dynamic) {
                        // update collision shape
                        rigid_body_comp.col_shape->calculateLocalInertia(rigid_body_comp.mass, local_inertia);
                    }

                    rigid_body_comp.motion_state = new btDefaultMotionState(start_transform);
                    btRigidBody::btRigidBodyConstructionInfo rb_info(rigid_body_comp.mass, rigid_body_comp.motion_state, rigid_body_comp.col_shape, local_inertia);
                    rigid_body_comp.rigid_body = new btRigidBody(rb_info);
                    
                    this->add_rigid_body(rigid_body_comp.rigid_body);
                    rigid_body_comp.initialized = true;

                    logger.debug("Initialized rigid body component");
                    return;
                }

                btRigidBody* rigid_body = rigid_body_comp.rigid_body;
                btTransform trans;
                if (rigid_body_comp.rigid_body->getMotionState()) {
                    rigid_body->getMotionState()->getWorldTransform(trans);
                    
                    trans.getOpenGLMatrix(glm::value_ptr(transform_comp.transform_matrix));
                }
            });
        }
    }
}