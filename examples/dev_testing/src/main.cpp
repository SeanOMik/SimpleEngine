#include "simpleengine/camera.h"
#include "simpleengine/gfx/light.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/vector.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/event/event.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/game.h>
#include <simpleengine/vertex.h>
#include <simpleengine/objects/3d/obj_model.h>
#include <simpleengine/gfx/shaders/core_3d_shader.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdint.h>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(resource_shaders);

#include <SOIL2/SOIL2.h>

namespace se = simpleengine;

std::string read_resource_shader(const std::string& path) {
    auto fs = cmrc::resource_shaders::get_filesystem();
    cmrc::file vertex_file = fs.open(path);

    return std::string(vertex_file.begin());
}

int main(int argc, char *argv[]) {
    se::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    /* se::gfx::Texture wall_texture("resources/wall.jpg");
    se::gfx::Texture crate_texture("resources/container.jpg", true, true); */

    // Load core shaders from simpleengine resources
    se::gfx::shaders::Core3dShader core_shader;

    auto light = std::make_shared<se::gfx::Light>(core_shader, glm::vec3(0.f, 0.f, -20.f), glm::vec3(1.f, 1.f, 1.f));
    game.add_event(light);

    se::gfx::Texture white_texture("examples/dev_testing/resources/white_texture.png");
    /* white_texture.shine_damper = 10;
    white_texture.reflectivity = 1; */
    auto dragon = std::make_shared<se::objects_3d::ObjModel>(game.get_window(), core_shader, white_texture, "examples/dev_testing/resources/dragon.obj");
    dragon->translate(0.f, -5.f, -25.f);
    game.add_event(dragon);

    /* se::gfx::Texture stall_texture("resources/stallTexture.png");
    auto stall = std::make_shared<se::objects_3d::ObjModel>(game.get_window(), core_shader, stall_texture, "resources/stall.obj");
    stall->translate(0.f, -4.f, -25.f);
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
    square->scale(.75f);
    game.add_event(square);

    std::vector<se::Vertex> tri_vertices = {
        { se::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // top right
        { se::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { se::Vectorf(0.f, 0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // bottom left
    };

    auto tri = std::make_shared<se::gfx::Model>(game.get_window(), core_shader, tri_vertices);
    tri->translate(-1.25f, 0.f, -1.f);
    tri->scale(.75f);
    game.add_event(tri); */

    /* std::vector<se::Vertex> cube_vertices = {
        { se::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { se::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { se::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { se::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { se::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { se::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { se::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { se::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { se::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },
    };

    std::vector<GLuint> cube_indicies = {
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
    };

    auto cube = std::make_shared<se::gfx::Model>(game.get_window(), core_shader, cube_vertices, cube_indicies);
    game.add_event(cube); */

    auto camera = std::make_shared<se::Camera>(game.get_window(), core_shader, 70, glm::vec3(0, 0, -10));
    game.add_event(camera);

    return game.run();
}