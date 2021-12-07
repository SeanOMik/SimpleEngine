#include "simpleengine/camera.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/shapes/2d/square.h"
#include "simpleengine/vector.h"
#include <glm/ext/matrix_clip_space.hpp>
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

/* class Triangle3D : public simpleengine::shapes_2d::Triangle {
public:
    glm::mat4 projection_matrix;
    //glm::mat4 view_matrix;

    const float fov = 70;
    const float near_plane = 0.1f;
    const float far_plane = 1000.f;

    Triangle3D(simpleengine::gfx::Shader shader, std::vector<simpleengine::Vertex> vertices) :
        simpleengine::shapes_2d::Triangle(shader, vertices) {

        projection_matrix = glm::perspective(glm::radians(fov), 640.f / 480.f, near_plane, far_plane);

        shader.use();
        shader.set_uniform_matrix_4f("projection_matrix", projection_matrix, false);
        //shader.set_uniform_matrix_4f("view_matrix", view_matrix, false);
        shader.unuse();
    }

    Triangle3D(std::shared_ptr<GLuint> shader_program, std::vector<simpleengine::Vertex> vertices) :
        simpleengine::shapes_2d::Triangle(shader_program, vertices) {

        
    }

    virtual void render(GLFWwindow* target) override {

    }
}; */

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

    std::vector<simpleengine::Vertex> vertices = {
        { simpleengine::Vectorf(-0.5f, -0.5f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f) }, // bottom left
        { simpleengine::Vectorf(0.5f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) }, // bottom right
        { simpleengine::Vectorf(0.f, 0.5f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 1.0f) }, // top
    };

    auto tri = std::make_shared<simpleengine::shapes_2d::Triangle>(base_shader_program, vertices);
    //tri->set_texture(wall_texture);
    game.add_event(tri);

    /* std::vector<simpleengine::Vertex> vertices = {
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

    std::vector<GLuint> indicies = {
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

    auto square = std::make_shared<simpleengine::shapes_2d::Square>(base_shader_program, vertices, indicies);
    //square->set_texture(crate_texture);
    game.add_event(square); */

    auto camera = std::make_shared<simpleengine::Camera>(game.get_window(), base_shader_program);
    game.add_event(camera);

    return game.run();
}