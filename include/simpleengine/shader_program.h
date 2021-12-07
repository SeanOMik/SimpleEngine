#pragma once

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#elif
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <GLFW/glfw3.h>

#include "event/event.h"
#include "gfx/shader.h"

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace simpleengine {
    class ShaderProgram : public simpleengine::Event {
    private:
        using super = simpleengine::Event;
    public:
        std::shared_ptr<GLuint> program;
        std::vector<gfx::Shader> shaders;

        ShaderProgram() : program(std::make_shared<GLuint>(glCreateProgram())) {
            
        }

        virtual ~ShaderProgram() {
            glDeleteProgram(*program);
        }

        /**
         * @brief Add a shader to this shader program. Also checks that its using the same `program` as this.
         * 
         * @see ShaderProgram::add_shader(const gfx::ShaderType& type, const std::string& shader_path)
         * @param shader The shader to add.
         * @return ShaderProgram& self
         */
        ShaderProgram& add_shader(gfx::Shader& shader) {
            if (shader.program != this->program) {
                throw std::runtime_error("The added shader does not have the same program as this shade program!");
            }

            shaders.push_back(shader);

            return *this;
        }

        /**
         * @brief Create and add a shader from a string to this shader program.
         * 
         * @param type The type of the shader.
         * @param shader_path The path of the shader.
         * @return ShaderProgram& self
         */
        ShaderProgram& add_shader_from_source(const gfx::ShaderType& type, std::string& shader_source) {
            gfx::Shader shd = gfx::Shader::from_source(program, type, shader_source);

            shaders.emplace_back(shd);

            return *this;
        }

        /**
         * @brief Create and add a shader from a filepath to this shader program.
         * 
         * @param type The type of the shader.
         * @param shader_path The path of the shader.
         * @return ShaderProgram& self
         */
        ShaderProgram& add_shader_from_path(const gfx::ShaderType& type, const std::string& shader_path) {
            gfx::Shader shd = gfx::Shader::from_filepath(program, type, shader_path);

            shaders.emplace_back(shd);

            return *this;
        }

        /**
         * @brief Link the shader program. Also removes unused shader resources.
         * 
         */
        void link() {
            if (shaders.empty()) {
                throw std::runtime_error("Shaders cannot be empty when running simpleengine::ShaderProgram::link()!");
            }

            glLinkProgram(*program);

            GLint success = false;
            glGetProgramiv(*program, GL_LINK_STATUS, &success);

            if (!success) {
                std::cerr << "Failed to link shader program!" << std::endl;
                throw gfx::ShaderException("Failed to link shader program!");
            }

            for (gfx::Shader& shader : shaders) {
                shader.delete_shader();
            }
        }

        virtual void update(const float& delta_time) {

        }

        virtual void render(GLFWwindow* target) {
            glUseProgram(*program);
        }
    };
}