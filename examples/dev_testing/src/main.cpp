//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#include "simpleengine/gfx/texture.h"
#include "simpleengine/shapes/2d/square.h"
#include <simpleengine/gfx/shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/event/event.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/game.h>
#include <simpleengine/shapes/2d/triangle.h>
#include <simpleengine/vertex.h>

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

    simpleengine::Texture wall_texture("resources/wall.jpg");
    simpleengine::Texture crate_texture("resources/container.jpg", true, true);

    std::vector<simpleengine::Vertex> vertices = {
        {glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f)}, // bottom left
        {glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f)}, // bottom right
        {glm::vec3(0.f, 0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f)}, // top
    };

    std::shared_ptr<simpleengine::shapes_2d::Triangle> tri(new simpleengine::shapes_2d::Triangle(base_shader_program, vertices));
    //tri->set_texture(wall_texture);
    game.add_event(tri);

    /* std::vector<simpleengine::Vertex> vertices = {
        { glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f) },
        { glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },
        { glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) },
        { glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f) },
    };

    std::vector<GLuint> indicies = {
        0, 1, 3,
        1, 2, 3
    };

    std::shared_ptr<simpleengine::Event> square(new simpleengine::shapes_2d::Square(base_shader_program, crate_texture, vertices, indicies));
    game.add_event(square); */

    return game.run();
}