#include "simpleengine/camera.h"
#include "simpleengine/ecs/component/box_collider_component.h"
#include "simpleengine/ecs/component/mesh_component.h"
#include "simpleengine/ecs/component/rigid_body_component.h"
#include "simpleengine/ecs/component/transform_component.h"
#include "simpleengine/ecs/entity.h"
#include "simpleengine/ecs/registry.h"
#include "simpleengine/gfx/light.h"
#include "simpleengine/gfx/material.h"
#include "simpleengine/gfx/mesh.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/renderer.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/vector.h"
#include <semaphore.h>
#include <simpleengine/ecs/component/model_component.h>
#include <simpleengine/ecs/component/rotating_component.h>
#include <simpleengine/event/event.h>
#include <simpleengine/game.h>
#include <simpleengine/gfx/model.h>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/gfx/shaders/core_3d_shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/ecs/system/scene_system.h>
#include <simpleengine/vertex.h>
#include <simpleengine/log/logger.h>
#include <simpleengine/physics/physics_system.h>

#include "entt/entity/fwd.hpp"

#include <assimp/material.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <sstream>
#include <stdint.h>

#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

namespace se = simpleengine;

class FPSCounterEvent : public se::Renderable {
public:
    double last_frame_time_input;
    int frame_count_input;

    double last_frame_time_tps;
    int frame_count_tps;

    double last_frame_time_render;
    int frame_count_render;

    FPSCounterEvent() : se::Renderable() {
        last_frame_time_input = glfwGetTime();
        frame_count_input = 0;

        last_frame_time_tps = glfwGetTime();
        frame_count_tps = 0;

        last_frame_time_render = glfwGetTime();
        frame_count_render = 0;
    }

    virtual void update(const float &delta_time) {
        double current_time = glfwGetTime();
        frame_count_tps++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_tps >= 1.0) {
            SE_DEBUG("performance", "Fixed update: {}tps", frame_count_tps);
            frame_count_tps = 0;
            last_frame_time_tps += 1.0;
        }
    }

    virtual void input_update(const float &delta_time) {
        double current_time = glfwGetTime();
        frame_count_input++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_input >= 1.0) {
            SE_DEBUG("performance", "Input:        {}tps", frame_count_input);
            frame_count_input = 0;
            last_frame_time_input += 1.0;

        }
    }

    virtual void render(const float& interpolate_alpha, const float& frame_time) {
        double current_time = glfwGetTime();
        frame_count_render++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_render >= 1.0) {
            double ms_per_frame = 1000 / (double)frame_count_render;

            SE_DEBUG("performance", "Render:       {}fps, {:.2f}ms/frame", frame_count_render, ms_per_frame);
            SE_DEBUG("performance", "-------------------------------");
            frame_count_render = 0;
            last_frame_time_render += 1.0;
        }
    }
};

int main(int argc, char *argv[]) {
    se::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    se::log::LoggerManager::set_level(spdlog::level::trace);

    // Load core shaders from SimpleEngine resources
    se::gfx::shaders::Core3dShader core_shader;

    // Create an entity registry
    auto registry = std::make_shared<se::ecs::Registry>();

    auto camera = std::make_shared<se::Camera>(game.get_window(), core_shader, 70, glm::vec3(-6, 0, 0));
    //game.add_event(camera);

    // Create a renderer
    auto renderer = std::make_shared<se::gfx::Renderer>(game.get_window(), core_shader, camera);
    renderer->initialize();

    // Create a Scene and give it the renderer
    auto scene = std::make_shared<se::ecs::system::SceneSystem>(registry, renderer, camera);
    game.add_renderable(scene); 

    // Create a Physics System for handling the physics
    auto physics_sys = std::make_shared<se::physics::PhysicsSystem>(registry);
    game.add_event(physics_sys);

    /* se::ecs::Entity other_e = scene->create_entity();
    other_e.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx",
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);
    auto &other_transform = other_e.add_component<se::TransformComponent>();
    other_transform.translate(5.f, 0.5f, 1.f);

    // Create an Entity in the Scene and add components to it.
    se::ecs::Entity entity = scene->create_entity();

    entity.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx", 
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);

    auto &transform_comp = entity.add_component<se::TransformComponent>();
    transform_comp.translate(4.f, 0.f, 0.f); */

    se::ecs::Entity brick_e = scene->create_entity();
    brick_e.add_component<se::ecs::ModelComponent>("examples/dev_testing/resources/bricks/bricks.fbx");
    brick_e.add_component<se::ecs::TransformComponent>(glm::vec3(6.f, 6.f, 0.f));
    brick_e.add_component<se::ecs::BoxColliderComponent>(1.f, 1.f, 1.f);
    brick_e.add_component<se::ecs::RigidBodyComponent>(1.f, se::Vectorf(6.f, 6.f, 0.1f));

    se::ecs::Entity floor = scene->create_entity();
    floor.add_component<se::ecs::ModelComponent>("examples/dev_testing/resources/ground/ground.fbx");
    floor.add_component<se::ecs::TransformComponent>(glm::vec3(6.f, -6.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f));
    floor.add_component<se::ecs::BoxColliderComponent>(1.f, 1.f, 1.f);
    floor.add_component<se::ecs::RigidBodyComponent>(0.f, se::Vectorf(6.f, -6.f, 0.f));

    auto light = std::make_shared<se::gfx::Light>(core_shader, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    game.add_event(light);

    // TODO: Fix, for some reason it crashes
    auto fps_counter = std::make_shared<FPSCounterEvent>();
    game.add_renderable(fps_counter);

    /* game.set_enable_vsync(false);
    game.set_fps_limit(100); */
    int res = game.run();

    SE_CINFO("Engine result: {}", res);

    renderer->destroy();
    scene->destroy();

    return res;
}