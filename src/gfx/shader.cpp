#include "gfx/shader.h"

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace simpleengine::gfx {
    Shader::Shader(std::shared_ptr<GLuint> program) : program(program) {

    }

    Shader::Shader(std::shared_ptr<GLuint> program, GLuint shader) : program(program), shader(shader) {
        
    }

    Shader Shader::from_source(const ShaderType& type, std::string& shader_source) {
        Shader shd = Shader::from_source(std::make_shared<GLuint>(glCreateProgram()), type, shader_source);

        shd.link();
        shd.delete_shader();

        return shd;
    }

    Shader Shader::from_source(std::shared_ptr<GLuint> program, const ShaderType& type, std::string& shader_source) {
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

        shd.attach(shd.program);

        return shd;
    }

    Shader Shader::from_filepath(const ShaderType& type, const std::string& shader_path) {
        Shader shd = Shader::from_filepath(std::make_shared<GLuint>(glCreateProgram()), type, shader_path);

        shd.link();
        shd.delete_shader();

        return shd;
    }

    Shader Shader::from_filepath(std::shared_ptr<GLuint> program, const ShaderType& type,
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

    Shader::~Shader() {
        delete_shader();
    }

    void Shader::link() {
        glLinkProgram(*program);

        GLint success = false;
        glGetProgramiv(*program, GL_LINK_STATUS, &success);

        if (!success) {
            std::cerr << "Failed to link shader program!" << std::endl;
            throw ShaderException("Failed to link shader program!");
        }
    }

    void Shader::delete_shader() {
        glDeleteShader(shader);
    }

    void Shader::use() const {
        glUseProgram(*program);
    }

    void Shader::unuse() {
        glUseProgram(0);
    }

    void Shader::attach(std::shared_ptr<GLuint> program) {
        glAttachShader(*program, shader);
    }

    GLfloat Shader::get_uniform_float(GLint location) const {
        use();

        GLfloat fl;
        glGetUniformfv(*program, location, &fl);

        return fl;
    }

    GLfloat Shader::get_uniform_float(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return get_uniform_float(location);
    }

    GLint Shader::get_uniform_int(GLint location) const {
        use();

        GLint _int;
        glGetUniformiv(*program, location, &_int);

        return _int;
    }

    GLint Shader::get_uniform_int(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return get_uniform_int(location);
    }

    GLuint Shader::get_uniform_uint(GLint location) const {
        use();

        GLuint _uint;
        glGetUniformuiv(*program, location, &_uint);

        return _uint;
    }

    GLuint Shader::get_uniform_uint(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return get_uniform_uint(location);
    }

    GLdouble Shader::get_uniform_double(GLint location) const {
        use();

        GLdouble dbl;
        glGetUniformdv(*program, location, &dbl);

        return dbl;
    }

    GLdouble Shader::get_uniform_double(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return get_uniform_double(location);
    }

    void Shader::set_uniform_float(GLint location, GLfloat fl, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1fv(location, 1, &fl);
        if (bind_shader) {
            unuse();
        }
    }
 
    void Shader::set_uniform_float(const char* uniform_name, GLfloat fl, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_float(location, fl, bind_shader);
    }

    void Shader::set_uniform_float_vec2(GLint location, glm::vec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2fv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_float_vec2(const char* uniform_name, glm::vec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_float_vec2(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_float_vec3(GLint location, glm::vec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3fv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_float_vec3(const char* uniform_name, glm::vec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_float_vec3(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_float_vec4(GLint location, glm::vec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4fv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_float_vec4(const char* uniform_name, glm::vec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_float_vec4(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_int(GLint location, GLint i, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1iv(location, 1, &i);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_int(const char* uniform_name, GLint i, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_int(location, i, bind_shader);
    }
    
    void Shader::set_uniform_int_vec2(GLint location, glm::ivec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2iv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_int_vec2(const char* uniform_name, glm::ivec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_int_vec2(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_int_vec3(GLint location, glm::ivec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3iv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_int_vec3(const char* uniform_name, glm::ivec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_int_vec3(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_int_vec4(GLint location, glm::ivec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4iv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_int_vec4(const char* uniform_name, glm::ivec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_int_vec4(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_uint(GLint location, GLuint ui, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1uiv(location, 1, &ui);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_uint(const char* uniform_name, GLuint ui, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_uint(location, ui, bind_shader);
    }
    
    void Shader::set_uniform_uint_vec2(GLint location, glm::uvec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2uiv(location, 1,glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_uint_vec2(const char* uniform_name, glm::uvec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_uint_vec2(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_uint_vec3(GLint location, glm::uvec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3uiv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_uint_vec3(const char* uniform_name, glm::uvec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_uint_vec3(location, vec, bind_shader);
    }
    
    void Shader::set_uniform_uint_vec4(GLint location, glm::uvec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4uiv(location, 1, glm::value_ptr(vec));
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::set_uniform_uint_vec4(const char* uniform_name, glm::uvec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        set_uniform_uint_vec4(location, vec, bind_shader);
    }
}