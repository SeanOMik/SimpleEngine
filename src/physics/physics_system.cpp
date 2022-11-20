#include "ecs/component/box_collider_component.h"
#include "ecs/component/sphere_collider_component.h"
#include "ecs/component/cylinder_collider_component.h"
#include "ecs/component/cone_collider_component.h"
#include "ecs/component/capsule_collider_component.h"
#include "ecs/component/rigid_body_component.h"
#include "ecs/system/system.h"
#include "entt/entity/fwd.hpp"
#include "physics/physics_system.h"
#include "ecs/component/transform_component.h"
#include "ecs/entity.h"
#include "ecs/registry.h"
#include "game.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

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

            entity_registry->get_inner().view<simpleengine::ecs::TransformComponent, simpleengine::ecs::RigidBodyComponent>()
                .each([this](const entt::entity& entity, simpleengine::ecs::TransformComponent& transform_comp, simpleengine::ecs::RigidBodyComponent& rigid_body_comp) {
                    // Initialize the rigid body component if it hasn't already been initialized.
                    init_rigid_body_component(entity, transform_comp, rigid_body_comp);

                    btRigidBody* rigid_body = rigid_body_comp.rigid_body;
                    btTransform trans;
                    if (rigid_body_comp.rigid_body->getMotionState()) {
                        rigid_body->getMotionState()->getWorldTransform(trans);
                        
                        trans.getOpenGLMatrix(glm::value_ptr(transform_comp.transform_matrix));
                    }
            });
        }
    }

    btCollisionShape* PhysicsSystem::try_get_collision_shape(const entt::entity& entity) {
        if (auto box_col = entity_registry->get_inner().try_get<simpleengine::ecs::BoxColliderComponent>(entity)) {
            return box_col->box_shape.get_inner_ptr();
        } else if (auto sphere_col = entity_registry->get_inner().try_get<simpleengine::ecs::SphereColliderComponent>(entity)) {
            return sphere_col->sphere_shape.get_inner_ptr();
        } else if (auto cylinder_col = entity_registry->get_inner().try_get<simpleengine::ecs::CylinderColliderComponent>(entity)) {
            return cylinder_col->cylinder_shape.get_inner_ptr();
        } else if (auto cone_col = entity_registry->get_inner().try_get<simpleengine::ecs::ConeColliderComponent>(entity)) {
            return cone_col->cone_shape.get_inner_ptr();
        } else if (auto capsule_col = entity_registry->get_inner().try_get<simpleengine::ecs::CapsuleColliderComponent>(entity)) {
            return capsule_col->capsule_shape.get_inner_ptr();
        }
        
        return nullptr;
    }

    bool PhysicsSystem::init_rigid_body_component(const entt::entity& entity, simpleengine::ecs::TransformComponent& transform_comp, simpleengine::ecs::RigidBodyComponent& rigid_body_comp) {
        if (!rigid_body_comp.rigid_body) {
            // Try to get the collision shape
            btCollisionShape* collision_shape = try_get_collision_shape(entity);
            if (!collision_shape) {
                logger.warn("Entity with a rigid body component is missing a collider! The entities rigid body component will be removed!");

                if (entity_registry->get_inner().remove<simpleengine::ecs::RigidBodyComponent>(entity) == 0) {
                    logger.warn("Failed to remove rigid body component from the entity!");
                }

                return true;
            }

            btTransform start_transform;
            start_transform.setIdentity();

            glm::vec3 origin = transform_comp.get_pos();
            start_transform.setOrigin(btVector3(origin.x, origin.y, origin.z));

            glm::quat rot = transform_comp.get_rotation_quat();
            start_transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

            rigid_body_comp.col_shape = collision_shape;

            btVector3 local_inertia;
            if (rigid_body_comp.is_dynamic) {
                // update collision shape
                rigid_body_comp.col_shape->calculateLocalInertia(rigid_body_comp.mass, local_inertia);
            }

            rigid_body_comp.motion_state = new btDefaultMotionState(start_transform);
            btRigidBody::btRigidBodyConstructionInfo rb_info(rigid_body_comp.mass, rigid_body_comp.motion_state, rigid_body_comp.col_shape, local_inertia);
            rigid_body_comp.rigid_body = new btRigidBody(rb_info);
            
            this->add_rigid_body(rigid_body_comp.rigid_body);

            logger.debug("Initialized rigid body component");
            return true;
        }

        return false;
    }
}