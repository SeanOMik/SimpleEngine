#include "gfx/shader.h"

#include <glm/fwd.hpp>
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

        glAttachShader(*shd.program, shd.shader);

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

    GLfloat Shader::getUniformFloat(GLint location) const {
        use();

        GLfloat fl;
        glGetUniformfv(*program, location, &fl);

        return fl;
    }

    GLfloat Shader::getUniformFloat(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return getUniformFloat(location);
    }

    GLint Shader::getUniformInt(GLint location) const {
        use();

        GLint _int;
        glGetUniformiv(*program, location, &_int);

        return _int;
    }

    GLint Shader::getUniformInt(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return getUniformInt(location);
    }

    GLuint Shader::getUniformUInt(GLint location) const {
        use();

        GLuint _uint;
        glGetUniformuiv(*program, location, &_uint);

        return _uint;
    }

    GLuint Shader::getUniformUInt(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return getUniformUInt(location);
    }

    GLdouble Shader::getUniformDouble(GLint location) const {
        use();

        GLdouble dbl;
        glGetUniformdv(*program, location, &dbl);

        return dbl;
    }

    GLdouble Shader::getUniformDouble(const char* uniform_name) const {
        int location = glGetUniformLocation(*program, uniform_name);
        return getUniformDouble(location);
    }

    void Shader::setUniformFloat(GLint location, GLfloat fl, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1f(location, fl);
        if (bind_shader) {
            unuse();
        }
    }
 
    void Shader::setUniformFloat(const char* uniform_name, GLfloat fl, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformFloat(location, fl, bind_shader);
    }

    void Shader::setUniformFloatVec2(GLint location, glm::vec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2f(location, vec.x, vec.y);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformFloatVec2(const char* uniform_name, glm::vec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformFloatVec2(location, vec, bind_shader);
    }
    
    void Shader::setUniformFloatVec3(GLint location, glm::vec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3f(location, vec.x, vec.y, vec.z);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformFloatVec3(const char* uniform_name, glm::vec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformFloatVec3(location, vec, bind_shader);
    }
    
    void Shader::setUniformFloatVec4(GLint location, glm::vec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformFloatVec4(const char* uniform_name, glm::vec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformFloatVec4(location, vec, bind_shader);
    }
    
    void Shader::setUniformInt(GLint location, GLint i, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1i(location, i);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformInt(const char* uniform_name, GLint i, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformInt(location, i, bind_shader);
    }
    
    void Shader::setUniformIntVec2(GLint location, glm::ivec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2i(location, vec.x, vec.y);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformIntVec2(const char* uniform_name, glm::ivec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformIntVec2(location, vec, bind_shader);
    }
    
    void Shader::setUniformIntVec3(GLint location, glm::ivec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3i(location, vec.x, vec.y, vec.z);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformIntVec3(const char* uniform_name, glm::ivec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformIntVec3(location, vec, bind_shader);
    }
    
    void Shader::setUniformIntVec4(GLint location, glm::ivec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformIntVec4(const char* uniform_name, glm::ivec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformIntVec4(location, vec, bind_shader);
    }
    
    void Shader::setUniformUInt(GLint location, GLuint ui, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform1ui(location, ui);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformUInt(const char* uniform_name, GLuint ui, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformUInt(location, ui, bind_shader);
    }
    
    void Shader::setUniformUIntVec2(GLint location, glm::uvec2 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform2ui(location, vec.x, vec.y);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformUIntVec2(const char* uniform_name, glm::uvec2 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformUIntVec2(location, vec, bind_shader);
    }
    
    void Shader::setUniformUIntVec3(GLint location, glm::uvec3 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform3ui(location, vec.x, vec.y, vec.z);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformUIntVec3(const char* uniform_name, glm::uvec3 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformUIntVec3(location, vec, bind_shader);
    }
    
    void Shader::setUniformUIntVec4(GLint location, glm::uvec4 vec, bool bind_shader) {
        if (bind_shader) {
            use();
        }
        glUniform4ui(location, vec.x, vec.y, vec.z, vec.w);
        if (bind_shader) {
            unuse();
        }
    }

    void Shader::setUniformUIntVec4(const char* uniform_name, glm::uvec4 vec, bool bind_shader) {
        int location = glGetUniformLocation(*program, uniform_name);
        setUniformUIntVec4(location, vec, bind_shader);
    }
}