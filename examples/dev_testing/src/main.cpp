#include "simpleengine/camera.h"
#include "simpleengine/gfx/light.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/renderer.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/objects/3d/terrain.h"
#include "simpleengine/vector.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/event/event.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/game.h>
#include <simpleengine/vertex.h>
#include <simpleengine/objects/3d/mesh.h>
#include <simpleengine/gfx/shaders/core_3d_shader.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdint.h>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(resource_shaders);

#ifdef __linux__
#include <SOIL2.h>
#else
#include <SOIL2/SOIL2.h>
#endif

namespace se = simpleengine;

class RendererEvent : public se::Event {
public:
    RendererEvent(se::gfx::Renderer renderer) : se::Event(), renderer(renderer) {}
    ~RendererEvent() = default;

    se::gfx::Renderer renderer;
    
    virtual void update(const float& delta_time) override {

    }

    virtual void render(GLFWwindow* target) override {
        this->renderer.render(nullptr);
    }
};

std::string read_resource_shader(const std::string& path) {
    auto fs = cmrc::resource_shaders::get_filesystem();
    cmrc::file vertex_file = fs.open(path);

    return std::string(vertex_file.begin());
}

int main(int argc, char *argv[]) {
    se::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    /* se::gfx::Texture wall_texture("resources/wall.jpg");
    se::gfx::Texture crate_texture("resources/container.jpg", true, true); */

    // Load core shaders from SimpleEngine resources
    se::gfx::shaders::Core3dShader core_shader;

    auto light = std::make_shared<se::gfx::Light>(core_shader, glm::vec3(0.f, 1.f, -10.f), glm::vec3(1.f, 1.f, 1.f));
    game.add_event(light);

    se::gfx::Texture white_texture("examples/dev_testing/resources/white_texture.png");
    // white_texture.shine_damper = 10;
    //white_texture.reflectivity = 1;
    /* auto dragon = std::make_shared<se::objects_3d::Mesh>(game.get_window(), core_shader, white_texture, "examples/dev_testing/resources/dragon.obj");
    dragon->translate(0.f, -5.f, -15.f);
    game.add_event(dragon); */

    /* auto cube = std::make_shared<se::objects_3d::Mesh>(game.get_window(), core_shader, white_texture, "examples/dev_testing/resources/cube.obj");
    cube->translate(0.f, -5.f, -15.f);
    game.add_event(cube);

    se::gfx::Texture grass("examples/dev_testing/resources/grass.png");
    auto terrain = std::make_shared<se::objects_3d::Terrain>(game.get_window(), core_shader, grass, 0, 0);
    terrain->translate(0.f, -5.f, -15.f);
    game.add_event(terrain); */

    /* se::gfx::Texture stall_texture("examples/dev_testing/resources/stallTextureb.png");
    auto stall = std::make_shared<se::objects_3d::Mesh>(game.get_window(), core_shader, stall_texture, "examples/dev_testing/resources/stall.obj");
    stall->translate(10.f, -5.f, 0.f);
    stall->rotate_y(90.f);
    game.add_event(stall); */

    /* std::vector<se::Vertex> square_vertices = {
        { se::Vectorf(0.5f, 0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // top right
        { se::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { se::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // bottom left
        { se::Vectorf(-0.5f, 0.5f, -1.f), glm::vec3(.5f, 0.5f, 0.f), glm::vec2(0.5f, 1.0f) }, // top left
    };

    std::vector<GLuint> indicies = {
        0, 1, 3,
        1, 2, 3
    };

    auto square = std::make_shared<se::gfx::Model>(game.get_window(), core_shader, square_vertices, indicies);
    square->translate(1.25f, 0.f, -1.f);
    //square->rotate_y(90.f);
    //square->scale(.75f);
    game.add_event(square); */

    /* se::gfx::Texture white_texture("examples/dev_testing/resources/white_texture.png");
    auto cube = std::make_shared<se::objects_3d::Mesh>(game.get_window(), core_shader, white_texture, "examples/dev_testing/resources/cube.obj");
    cube->translate(-1.25f, 0.f, -1.f);
    game.add_event(cube); */

    /* std::vector<se::Vertex> tri_vertices = {
        { se::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // top right
        { se::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { se::Vectorf(0.f, 0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // bottom left
    };

    auto tri = std::make_shared<se::gfx::Model>(game.get_window(), core_shader, tri_vertices);
    tri->translate(-1.25f, 0.f, -1.f);
    tri->scale(.75f);
    game.add_event(tri); */

    std::vector<se::LitVertex> cube_vertices = {
        { se::Vectorf(-1.f, -1.f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(1.f, -1.f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(2.f, 0.f) },
        { se::Vectorf(-1.f, 1.f, -1.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(3.f, 0.f) },
        { se::Vectorf(-1, -1, -1), glm::vec3(1.f, 1.f, 0.f), glm::vec2(4.f, 0.f) },

        { se::Vectorf(-1, -1, 1), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f) },
        { se::Vectorf(1, -1, 1), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(1, 1, 1), glm::vec3(0.f, 0.f, 1.f), glm::vec2(2.f, 1.f) },
        { se::Vectorf(-1, 1, 1), glm::vec3(1.f, 1.f, 0.f), glm::vec2(3.f, 1.f) },
        { se::Vectorf(-1, -1, 1), glm::vec3(1.f, 1.f, 0.f), glm::vec2(4.f, 1.f) },

        { se::Vectorf(-1, 1, -1), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, -1.f) },
        { se::Vectorf(1, 1, -1), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, -1.f) },

        { se::Vectorf(-1, 1, 1), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 2.f) },
        { se::Vectorf(1, 1, 1), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 2.f) },
    };

    std::vector<GLuint> cube_indicies = {
        0, 1, 5, 5, 1, 6,
        1, 2, 6, 6, 2, 7,
        2, 3, 7, 7, 3, 8,
        3, 4, 8, 8, 4, 9,
        10, 11, 0, 0, 11, 1,
        5, 6, 12, 12, 6, 13
    };

    auto cube = std::make_shared<se::gfx::Model>(game.get_window(), core_shader, cube_vertices, cube_indicies);
    cube->calculate_normals();
    cube->translate(3.5f, 0.f, 0.f);
    //game.add_event(cube);

    auto renderer  = std::make_shared<se::gfx::Renderer>(game.get_window(), core_shader);
    renderer->add_model(white_texture, cube);
    game.add_event(renderer);

    /* auto r_event = std::make_shared<RendererEvent>(renderer);
    game.add_event(r_event); */

    auto camera = std::make_shared<se::Camera>(game.get_window(), core_shader, 70, glm::vec3(0, 0, 0));
    game.add_event(camera);

    return game.run();
}