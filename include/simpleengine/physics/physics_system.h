#pragma once

#include "../vector.h"
#include "../ecs/entity.h"
#include "../log/logger.h"
#include "../ecs/system/system.h"

#include <memory>

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;

namespace simpleengine {
    namespace ecs {
        class Registry;
    }

    namespace physics {
        class PhysicsSystem : public simpleengine::ecs::system::System {
        protected:
            std::unique_ptr<btDefaultCollisionConfiguration> collision_configuration;
            std::unique_ptr<btCollisionDispatcher> col_dispatcher;
            std::unique_ptr<btBroadphaseInterface> overlapping_pair_cache;
            std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
            std::unique_ptr<btDiscreteDynamicsWorld> dynamics_world;

            simpleengine::log::Logger logger;
        public:
            simpleengine::Vectorf gravity_vector;
            bool should_simulate;

            PhysicsSystem(std::shared_ptr<ecs::Registry> entity_registry);
            ~PhysicsSystem();

            void set_y_gravity(const float& gravity);

            /**
             * @brief Add a *manually* constructed rigid body to the Physics System.
             * 
             * @note Do not add a rigid body from a rigid body component. This will be added automatically by the Physics System.
             *
             * @param rigid_body The rigid body that *manually* created.
             */
            void add_rigid_body(btRigidBody* rigid_body);

            virtual void update(const float& delta_time) override;

            // Empty implementations
            virtual void input_update(const float& delta_time) override {}
            virtual void render(const float& interpolate_alpha, const float& frame_time) override {}
            virtual void destroy() override {}
        };
    }
}