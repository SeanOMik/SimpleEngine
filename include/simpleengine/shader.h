//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_SHADER_H
#define SIMPLEENGINE_SHADER_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include "event/event.h"

#include <exception>
#include <fstream>
#include <gl/gl.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>

namespace simpleengine {
    class ShaderException : public std::runtime_error {
    public:
        explicit ShaderException(char const* const msg) : std::runtime_error(msg) {

        }
    };

    enum ShaderType {
        ST_Vertex = GL_VERTEX_SHADER,
        ST_Fragment = GL_FRAGMENT_SHADER,
    };

    class Shader : public simpleengine::Event {
    private:
        using super = simpleengine::Event;
    protected:
        Shader() {

        }
    public:
        static Shader from_source(const ShaderType& type, std::string& shader_source) {
            Shader shd = Shader::from_source(std::make_shared<GLuint>(glCreateProgram()), type, shader_source);

            shd.link();
            shd.delete_shader();

            return shd;
        }

        static Shader from_source(std::shared_ptr<GLuint> program, const ShaderType& type, std::string& shader_source) {
            Shader shd;
            shd.program = program;
            shd.shader = glCreateShader(type);

            const GLchar* vert_src = shader_source.c_str();
            glShaderSource(shd.shader, 1, &vert_src, NULL);
            glCompileShader(shd.shader);

            GLint success = false;
            glGetShaderiv(shd.shader, GL_COMPILE_STATUS, &success);

            if (!success) {
                char log[512];
                glGetShaderInfoLog(shd.shader, 512, NULL, log);

                std::cerr << "Failed to load shader from source:" << std::endl << log << std::endl;
                throw ShaderException("Failed to compile shader!");
            }

            glAttachShader(*shd.program, shd.shader);

            return shd;
        }

        /**
         * @brief Load a shader from a filepath.
         * 
         * @param type The type of the shader.
         * @param shader_path The path of the shader source.
         */
        static Shader from_filepath(const ShaderType& type, const std::string& shader_path) {
            Shader shd = Shader::from_filepath(std::make_shared<GLuint>(glCreateProgram()), type, shader_path);

            shd.link();
            shd.delete_shader();

            return shd;
        }

        /**
         * @brief Load a shader from a filepath.
         * 
         * @param program The shader program that this shader will be attached to.
         * @param type The type of the shader.
         * @param shader_path The path of shader source.
         */
        static Shader from_filepath(std::shared_ptr<GLuint> program, const ShaderType& type,
                const std::string& shader_path) {
            std::ifstream fstream(shader_path, std::ios::in);

            if (!fstream.is_open()) {
                std::cerr << "Failed to open shader file: " << shader_path << std::endl;
                throw ShaderException("Failed to open shader file!");
            }

            std::stringstream ss;
            { 
                std::string str; 
                while (std::getline(fstream, str))
                {
                    ss << str << std::endl;
                }
            }
            std::string ss_str = ss.str();
            
            return Shader::from_source(type, ss_str);
        }

        virtual ~Shader() {
            delete_shader();
        }

        /**
         * @brief Link the shader program and checks for errors.
         * 
         */
        void link() {
            glLinkProgram(*program);

            GLint success = false;
            glGetProgramiv(*program, GL_LINK_STATUS, &success);

            if (!success) {
                std::cerr << "Failed to link shader program!" << std::endl;
                throw ShaderException("Failed to link shader program!");
            }
        }
        
        /**
         * @brief Delete the shader with glDeleteShader. Only do this after we've linked.
         * 
         */
        void delete_shader() {
            glDeleteShader(shader);
        }

        virtual void update(const float& delta_time) override {
            //super::update(delta_time);
        }

        virtual void render(std::shared_ptr<GLFWwindow> target) override {
            //super::render(target);
        }

        std::shared_ptr<GLuint> program;
    private:
        GLuint shader;
    };
}

#endif //SIMPLEENGINE_SHADER_H