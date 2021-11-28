#pragma once

#include <chrono>
#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../event/event.h"

#include <exception>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>

namespace simpleengine::gfx {
    class ShaderException : public std::runtime_error {
    public:
        explicit ShaderException(char const* const msg) : std::runtime_error(msg) {

        }
    };

    enum ShaderType {
        ST_Vertex = GL_VERTEX_SHADER,
        ST_Fragment = GL_FRAGMENT_SHADER,
    };

    class Shader {
    protected:
        Shader() {

        }
    public:
        std::shared_ptr<GLuint> program;
        GLuint shader;

        Shader(std::shared_ptr<GLuint> program);

        Shader(std::shared_ptr<GLuint> program, GLuint shader);

        static Shader from_source(const ShaderType& type, std::string& shader_source);

        static Shader from_source(std::shared_ptr<GLuint> program, const ShaderType& type, std::string& shader_source);

        /**
         * @brief Load a shader from a filepath.
         * 
         * @param type The type of the shader.
         * @param shader_path The path of the shader source.
         */
        static Shader from_filepath(const ShaderType& type, const std::string& shader_path);

        /**
         * @brief Load a shader from a filepath.
         * 
         * @param program The shader program that this shader will be attached to.
         * @param type The type of the shader.
         * @param shader_path The path of shader source.
         */
        static Shader from_filepath(std::shared_ptr<GLuint> program, const ShaderType& type, const std::string& shader_path);

        virtual ~Shader();

        /**
         * @brief Link the shader program and checks for errors.
         * 
         */
        void link();
        
        /**
         * @brief Delete the shader with glDeleteShader. Only do this after we've linked.
         * 
         */
        void delete_shader();

        /**
         * @brief Use the shader program.
         * 
         */
        void use() const;

        /**
         * @brief Un-use the shader program by using the id of "0".
         * 
         */
        static void unuse();

        /**
         * @brief Attach the shader to a program. No need to call this if its already attached once.
         * 
         * @param program The program to attach the shader to.
         */
        void attach(std::shared_ptr<GLuint> program);

        /**
         * @brief Get a Uniform Float from the shader using a `location`.
         * 
         * @param location The location of the uniform float.
         * @return GLfloat The value of the uniform float from the shader.
         */
        GLfloat get_uniform_float(GLint location) const;

        /**
         * @brief Get a Uniform Float from the shader finding the location of the uniform using `uniform_name`.
         * 
         * @param uniform_name The name of the uniform inside of the shader.
         * @return GLfloat The value of the uniform float from the shader.
         */
        GLfloat get_uniform_float(const char* uniform_name) const;

        /**
         * @brief Get a Uniform integer from the shader using a `location`.
         * 
         * @param location The location of the uniform integer.
         * @return GLint The value of the uniform integer from the shader.
         */
        GLint get_uniform_int(GLint location) const;

        /**
         * @brief Get a Uniform integer from the shader finding the location of the uniform using `uniform_name`.
         * 
         * @param uniform_name The name of the uniform inside of the shader.
         * @return GLint The value of the uniform integer from the shader.
         */
        GLint get_uniform_int(const char* uniform_name) const;

        /**
         * @brief Get a Uniform unsigned integer from the shader using a `location`.
         * 
         * @param location The location of the uniform unsigned integer.
         * @return GLuint The value of the uniform unsigned integer from the shader.
         */
        GLuint get_uniform_uint(GLint location) const;

        /**
         * @brief Get a Uniform unsigned integer from the shader finding the location of the uniform using `uniform_name`.
         * 
         * @param uniform_name The name of the uniform inside of the shader.
         * @return GLuint The value of the uniform unsigned integer from the shader.
         */
        GLuint get_uniform_uint(const char* uniform_name) const;

        /**
         * @brief Get a Uniform double from the shader using a `location`.
         * 
         * @param location The location of the uniform double.
         * @return GLdouble The value of the uniform double from the shader.
         */
        GLdouble get_uniform_double(GLint location) const;

        /**
         * @brief Get a Uniform double from the shader finding the location of the uniform using `uniform_name`.
         * 
         * @param uniform_name The name of the uniform inside of the shader.
         * @return GLdouble The value of the uniform double from the shader.
         */
        GLdouble get_uniform_double(const char* uniform_name) const;

        void set_uniform_float(GLint location, GLfloat fl, bool bind_shader = true);
        void set_uniform_float(const char* uniform_name, GLfloat fl, bool bind_shader = true);
 
        void set_uniform_float_vec2(GLint location, glm::vec2 vec, bool bind_shader = true);
        void set_uniform_float_vec2(const char* uniform_name, glm::vec2 vec, bool bind_shader = true);

        void set_uniform_float_vec3(GLint location, glm::vec3 vec, bool bind_shader = true);
        void set_uniform_float_vec3(const char* uniform_name, glm::vec3 vec, bool bind_shader = true);
        
        void set_uniform_float_vec4(GLint location, glm::vec4 vec, bool bind_shader = true);
        void set_uniform_float_vec4(const char* uniform_name, glm::vec4 vec, bool bind_shader = true);
        
        void set_uniform_int(GLint location, GLint i, bool bind_shader = true);
        void set_uniform_int(const char* uniform_name, GLint i, bool bind_shader = true);
        
        void set_uniform_int_vec2(GLint location, glm::ivec2 vec, bool bind_shader = true);
        void set_uniform_int_vec2(const char* uniform_name, glm::ivec2 vec, bool bind_shader = true);
        
        void set_uniform_int_vec3(GLint location, glm::ivec3 vec, bool bind_shader = true);
        void set_uniform_int_vec3(const char* uniform_name, glm::ivec3 vec, bool bind_shader = true);
        
        void set_uniform_int_vec4(GLint location, glm::ivec4 vec, bool bind_shader = true);
        void set_uniform_int_vec4(const char* uniform_name, glm::ivec4 vec, bool bind_shader = true);
        
        void set_uniform_uint(GLint location, GLuint ui, bool bind_shader = true);
        void set_uniform_uint(const char* uniform_name, GLuint ui, bool bind_shader = true);
        
        void set_uniform_uint_vec2(GLint location, glm::uvec2 vec, bool bind_shader = true);
        void set_uniform_uint_vec2(const char* uniform_name, glm::uvec2 vec, bool bind_shader = true);
        
        void set_uniform_uint_vec3(GLint location, glm::uvec3 vec, bool bind_shader = true);
        void set_uniform_uint_vec3(const char* uniform_name, glm::uvec3 vec, bool bind_shader = true);
        
        void set_uniform_uint_vec4(GLint location, glm::uvec4 vec, bool bind_shader = true);
        void set_uniform_uint_vec4(const char* uniform_name, glm::uvec4 vec, bool bind_shader = true);
    };
}