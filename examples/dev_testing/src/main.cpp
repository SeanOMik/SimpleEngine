#include "simpleengine/camera.h"
#include "simpleengine/ecs/component/mesh_component.h"
#include "simpleengine/ecs/component/transform_component.h"
#include "simpleengine/ecs/entity.h"
#include "simpleengine/gfx/light.h"
#include "simpleengine/gfx/material.h"
#include "simpleengine/gfx/mesh.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/renderer.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/vector.h"
#include <simpleengine/ecs/component/model_component.h>
#include <simpleengine/ecs/component/rotating_component.h>
#include <simpleengine/event/event.h>
#include <simpleengine/game.h>
#include <simpleengine/gfx/model.h>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/gfx/shaders/core_3d_shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/scene.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/vertex.h>

#include <assimp/material.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdint.h>

namespace se = simpleengine;

class FPSCounterEvent : public se::Event {
public:
    double last_frame_time;
    int frame_count;

    FPSCounterEvent() : se::Event() {
        this->last_frame_time = glfwGetTime();
        frame_count = 0;
    }

    virtual void update(const float &delta_time) {
        double current_time = glfwGetTime();
        frame_count++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time >= 1.0) {
            double ms_per_frame = 1000 / (double)frame_count;

            printf("%d fps, %f ms/frame\n", frame_count, ms_per_frame);
            frame_count = 0;
            last_frame_time += 1.0;
        }
    }
};

int main(int argc, char *argv[]) {
    se::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    // Load core shaders from SimpleEngine resources
    se::gfx::shaders::Core3dShader core_shader;

    auto camera = std::make_shared<se::Camera>(game.get_window(), core_shader, 70, glm::vec3(0, 0, 0));
    game.add_event(camera);

    // Create a renderer
    auto renderer = std::make_shared<se::gfx::Renderer>(game.get_window(), core_shader, camera);
    renderer->initialize();
    //game.add_renderable(renderer);

    // Create a Scene and give it the renderer
    auto scene = std::make_shared<se::Scene>(renderer);
    //game.add_event(scene);
    game.add_renderable(scene); 

    se::ecs::Entity other_e = scene->create_entity();
    other_e.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx",
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);
    auto &other_transform = other_e.add_component<se::TransformComponent>();
    other_transform.translate(5.f, 0.5f, 1.f);

    // Create an Entity in the Scene and add components to it.
    se::ecs::Entity entity = scene->create_entity();

    // entity.add_component<se::ModelComponent>("examples/dev_testing/resources/planks/planks.fbx", simpleengine::gfx::ModelProcessingFlags::MdlProcFlag_CALCULATE_TANGENT_SPACE);
    //entity.add_component<se::ModelComponent>("examples/dev_testing/resources/bricks/bricks.fbx", simpleengine::gfx::ModelProcessingFlags::MdlProcFlag_CALCULATE_TANGENT_SPACE);
    entity.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx", 
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);

    auto &transform_comp = entity.add_component<se::TransformComponent>();
    transform_comp.translate(4.f, 0.f, 0.f);

    se::ecs::Entity brick_e = scene->create_entity();
    brick_e.add_component<se::ModelComponent>("examples/dev_testing/resources/bricks/bricks.fbx");
    brick_e.add_component<se::RotatingComponent>();
    auto &brick_transf = brick_e.add_component<se::TransformComponent>();
    brick_transf.translate(6.f, -0.5f, 1.f);

    auto light = std::make_shared<se::gfx::Light>(core_shader, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    game.add_event(light);

    auto fps_counter = std::make_shared<FPSCounterEvent>();
    game.add_event(fps_counter);

    game.set_enable_vsync(true);
    // game.set_fps_limit(120);
    int res = game.run();
    std::cout << "Engine result: " << res << std::endl;

    renderer->destroy();
    scene->destroy();

    return res;
}