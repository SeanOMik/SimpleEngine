#pragma once

#include "../shader.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(simpleengine_resources);

namespace simpleengine::gfx::shaders {
    class Core3dShader : public simpleengine::gfx::Shader {
    private:
        gfx::Shader fragment_shader;
    public:
        Core3dShader() {
            this->program = glCreateProgram();

            auto vertex_fs = cmrc::simpleengine_resources::get_filesystem();
            cmrc::file vertex_file = vertex_fs.open("resources/shaders/vertex_core.glsl");
            std::string vertex_source(vertex_file.begin());
            Shader vertex = gfx::Shader::from_source(program, ShaderType::ST_Vertex, vertex_source);
            this->shader = vertex.shader;

            auto fragment_fs = cmrc::simpleengine_resources::get_filesystem();
            cmrc::file fragment_file = fragment_fs.open("resources/shaders/fragment_core.glsl");
            std::string fragment_source(fragment_file.begin());
            fragment_shader = gfx::Shader::from_source(program, ShaderType::ST_Fragment, fragment_source);

            link();
            delete_shader();
            fragment_shader.delete_shader();
        }
    };
}