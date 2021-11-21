//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#include <simpleengine/shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/event/event.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/game.h>
#include <simpleengine/shapes/2d/triangle.h>

#include <chrono>
#include <iostream>
#include <sstream>

#include <cmrc/cmrc.hpp>
#include <stdint.h>
CMRC_DECLARE(resource_shaders);

std::string read_resource_shader(const std::string& path) {
    auto fs = cmrc::resource_shaders::get_filesystem();
    cmrc::file vertex_file = fs.open(path);

    return std::string(vertex_file.begin());
}

int main(int argc, char *argv[]) {
    simpleengine::Game game(1280, 720, "SimpleEngine 3D OpenGL - Developer Testing", false);

    // Load shaders
    std::string vertex_core = read_resource_shader("shaders/vertex_core.glsl");
    std::string fragment_core = read_resource_shader("shaders/fragment_core.glsl");

    // Create shader program
    simpleengine::ShaderProgram shader_prog;
    shader_prog.add_shader_from_source(simpleengine::ShaderType::Vertex, vertex_core);
    shader_prog.add_shader_from_source(simpleengine::ShaderType::Fragment, fragment_core);
    shader_prog.link();
    std::shared_ptr<GLuint> base_shader_program = shader_prog.program;

    // Create just a simple 2d triangle
    std::shared_ptr<simpleengine::Event> tri(new simpleengine::shapes_2d::Triangle(base_shader_program));
    game.add_event(tri);

    return game.run();
}