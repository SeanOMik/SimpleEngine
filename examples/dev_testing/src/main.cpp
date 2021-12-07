#include "simpleengine/camera.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/vector.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/event/event.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/game.h>
#include <simpleengine/vertex.h>
#include <simpleengine/objects/3d/obj_model.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdint.h>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(resource_shaders);

#include <SOIL2/SOIL2.h>

std::string read_resource_shader(const std::string& path) {
    auto fs = cmrc::resource_shaders::get_filesystem();
    cmrc::file vertex_file = fs.open(path);

    return std::string(vertex_file.begin());
}

int main(int argc, char *argv[]) {
    simpleengine::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    // Load shaders
    std::string vertex_core = read_resource_shader("shaders/vertex_core.glsl");
    std::string fragment_core = read_resource_shader("shaders/fragment_core.glsl");

    // Create shader program
    simpleengine::ShaderProgram shader_prog;
    shader_prog.add_shader_from_source(simpleengine::gfx::ShaderType::ST_Vertex, vertex_core);
    shader_prog.add_shader_from_source(simpleengine::gfx::ShaderType::ST_Fragment, fragment_core);
    shader_prog.link();
    std::shared_ptr<GLuint> base_shader_program = shader_prog.program;

    /* simpleengine::gfx::Texture wall_texture("resources/wall.jpg");
    simpleengine::gfx::Texture crate_texture("resources/container.jpg", true, true); */

    simpleengine::gfx::Texture stall_texture("resources/stallTexture.png");

    auto stall = std::make_shared<simpleengine::objects_3d::ObjModel>(game.get_window(), simpleengine::gfx::Shader(base_shader_program),
        stall_texture, "resources/stall.obj");
    stall->set_texture(stall_texture);
    stall->translate(0.f, -4.f, -18.f);
    game.add_event(stall);

    /* std::vector<simpleengine::Vertex> square_vertices = {
        { simpleengine::Vectorf(0.5f, 0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // top right
        { simpleengine::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { simpleengine::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // bottom left
        { simpleengine::Vectorf(-0.5f, 0.5f, -1.f), glm::vec3(.5f, 0.5f, 0.f), glm::vec2(0.5f, 1.0f) }, // top left
    };

    std::vector<GLuint> indicies = {
        0, 1, 3,
        1, 2, 3
    };

    auto square = std::make_shared<simpleengine::gfx::Model>(game.get_window(), base_shader_program, square_vertices, indicies);
    square->translate(1.25f, 0.f, -1.f);
    square->scale(.75f);
    game.add_event(square);

    std::vector<simpleengine::Vertex> tri_vertices = {
        { simpleengine::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // top right
        { simpleengine::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { simpleengine::Vectorf(0.f, 0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // bottom left
    };

    auto tri = std::make_shared<simpleengine::gfx::Model>(game.get_window(), base_shader_program, tri_vertices);
    tri->translate(-1.25f, 0.f, -1.f);
    tri->scale(.75f);
    game.add_event(tri); */

    /* std::vector<simpleengine::Vertex> cube_vertices = {
        { simpleengine::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { simpleengine::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { simpleengine::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { simpleengine::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { simpleengine::Vectorf(-0.5f,0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(-0.5f,0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(0.5f,0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(0.5f,0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },

        { simpleengine::Vectorf(-0.5f,-0.5f,0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { simpleengine::Vectorf(-0.5f,-0.5f,-0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,-0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { simpleengine::Vectorf(0.5f,-0.5f,0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },
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

    auto cube = std::make_shared<simpleengine::gfx::Model>(game.get_window(), base_shader_program, cube_vertices, cube_indicies);
    game.add_event(cube); */

    auto camera = std::make_shared<simpleengine::Camera>(game.get_window(), base_shader_program);
    game.add_event(camera);

    return game.run();
}