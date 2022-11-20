#include "gfx/shader.h"
#include "log/logger.h"

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace simpleengine::gfx {
    GLuint Shader::inuse_program = 0;

    Shader::Shader(GLuint program) : program(program) {

    }

    Shader::Shader(GLuint program, GLuint shader) : program(program), shader(shader) {
        
    }

    Shader Shader::from_source(const ShaderType& type, std::string& shader_source) {
        Shader shd = Shader::from_source(glCreateProgram(), type, shader_source);

        shd.link();
        shd.delete_shader();

        return shd;
    }

    Shader Shader::from_source(GLuint program, const ShaderType& type, std::string& shader_source) {
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

            SE_CERROR("Failed to load shader from source:\n{}", log);
            throw ShaderException("Failed to compile shader!");
        }

        shd.attach(shd.program);

        return shd;
    }

    Shader Shader::from_filepath(const ShaderType& type, const std::string& shader_path) {
        Shader shd = Shader::from_filepath(glCreateProgram(), type, shader_path);

        shd.link();
        shd.delete_shader();

        return shd;
    }

    Shader Shader::from_filepath(GLuint program, const ShaderType& type,
            const std::string& shader_path) {
        std::ifstream fstream(shader_path, std::ios::in);

        if (!fstream.is_open()) {
            SE_CERROR("Failed to open shader file: {}", shader_path);
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
        glLinkProgram(program);

        GLint success = false;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            SE_CERROR("Failed to link shader program!");
            throw ShaderException("Failed to link shader program!");
        }
    }

    void Shader::delete_shader() {
        glDeleteShader(shader);
    }

    void Shader::delete_program() {
        unuse();
        glDeleteProgram(program);
    }

    void Shader::use() const {
        if (Shader::inuse_program != program) {
            glUseProgram(program);

            Shader::inuse_program = program;
        }
    }

    void Shader::unuse() {
        glUseProgram(0);

        Shader::inuse_program = 0;
    }

    void Shader::attach(GLuint program) {
        glAttachShader(program, shader);
    }

    int Shader::get_uniform_location(const std::string& uniform_name) const {
        return this->get_uniform_location(uniform_name.c_str());
    }

    int Shader::get_uniform_location(const char* uniform_name) const {
        return glGetUniformLocation(program, uniform_name);
    }

    GLfloat Shader::get_uniform_float(GLint location) const {
        use();

        GLfloat fl;
        glGetUniformfv(program, location, &fl);

        return fl;
    }

    GLfloat Shader::get_uniform_float(const char* uniform_name) const {
        int location = get_uniform_location(uniform_name);
        return get_uniform_float(location);
    }

    GLint Shader::get_uniform_int(GLint location) const {
        use();

        GLint _int;
        glGetUniformiv(program, location, &_int);

        return _int;
    }

    GLint Shader::get_uniform_int(const char* uniform_name) const {
        int location = get_uniform_location(uniform_name);
        return get_uniform_int(location);
    }

    GLuint Shader::get_uniform_uint(GLint location) const {
        use();

        GLuint _uint;
        glGetUniformuiv(program, location, &_uint);

        return _uint;
    }

    GLuint Shader::get_uniform_uint(const char* uniform_name) const {
        int location = get_uniform_location(uniform_name);
        return get_uniform_uint(location);
    }

    GLdouble Shader::get_uniform_double(GLint location) const {
        use();

        GLdouble dbl;
        glGetUniformdv(program, location, &dbl);

        return dbl;
    }

    GLdouble Shader::get_uniform_double(const char* uniform_name) const {
        int location = get_uniform_location(uniform_name);
        return get_uniform_double(location);
    }

    void Shader::set_uniform_float(GLint location, GLfloat fl) {
        use();
        glUniform1fv(location, 1, &fl);
    }
 
    void Shader::set_uniform_float(const char* uniform_name, GLfloat fl) {
        int location = get_uniform_location(uniform_name);
        set_uniform_float(location, fl);
    }

    void Shader::set_uniform_float_array(GLint location, int count, GLfloat* arr) {
        use();
        glUniform1fv(location, count, arr);
    }

    void Shader::set_uniform_float_array(const char* uniform_name, int count, GLfloat* arr) {
        int location = get_uniform_location(uniform_name);
        set_uniform_float_array(location, count, arr);
    }
 

    void Shader::set_uniform_float_vec2(GLint location, glm::vec2 vec) {
        use();
        glUniform2fv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_float_vec2(const char* uniform_name, glm::vec2 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_float_vec2(location, vec);
    }
    
    void Shader::set_uniform_float_vec3(GLint location, glm::vec3 vec) {
        use();
        glUniform3fv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_float_vec3(const char* uniform_name, glm::vec3 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_float_vec3(location, vec);
    }
    
    void Shader::set_uniform_float_vec4(GLint location, glm::vec4 vec) {
        use();
        glUniform4fv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_float_vec4(const char* uniform_name, glm::vec4 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_float_vec4(location, vec);
    }
    
    void Shader::set_uniform_int(GLint location, GLint i) {
        use();
        glUniform1iv(location, 1, &i);
    }

    void Shader::set_uniform_int(const char* uniform_name, GLint i) {
        int location = get_uniform_location(uniform_name);
        set_uniform_int(location, i);
    }
    
    void Shader::set_uniform_int_array(GLint location, int count, GLint* arr) {
        use();
        glUniform1iv(location, count, arr);
    }

    void Shader::set_uniform_int_array(const char* uniform_name, int count, GLint* arr) {
        int location = get_uniform_location(uniform_name);
        set_uniform_int_array(location, count, arr);
    }

    void Shader::set_uniform_int_vec2(GLint location, glm::ivec2 vec) {
        use();
        glUniform2iv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_int_vec2(const char* uniform_name, glm::ivec2 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_int_vec2(location, vec);
    }
    
    void Shader::set_uniform_int_vec3(GLint location, glm::ivec3 vec) {
        use();
        glUniform3iv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_int_vec3(const char* uniform_name, glm::ivec3 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_int_vec3(location, vec);
    }
    
    void Shader::set_uniform_int_vec4(GLint location, glm::ivec4 vec) {
        use();
        glUniform4iv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_int_vec4(const char* uniform_name, glm::ivec4 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_int_vec4(location, vec);
    }
    
    void Shader::set_uniform_uint(GLint location, GLuint ui) {
        use();
        glUniform1uiv(location, 1, &ui);
    }

    void Shader::set_uniform_uint(const char* uniform_name, GLuint ui) {
        int location = get_uniform_location(uniform_name);
        set_uniform_uint(location, ui);
    }
    
    void Shader::set_uniform_uint_vec2(GLint location, glm::uvec2 vec) {
        use();
        glUniform2uiv(location, 1,glm::value_ptr(vec));
    }

    void Shader::set_uniform_uint_vec2(const char* uniform_name, glm::uvec2 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_uint_vec2(location, vec);
    }
    
    void Shader::set_uniform_uint_vec3(GLint location, glm::uvec3 vec) {
        use();
        glUniform3uiv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_uint_vec3(const char* uniform_name, glm::uvec3 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_uint_vec3(location, vec);
    }
    
    void Shader::set_uniform_uint_vec4(GLint location, glm::uvec4 vec) {
        use();
        glUniform4uiv(location, 1, glm::value_ptr(vec));
    }

    void Shader::set_uniform_uint_vec4(const char* uniform_name, glm::uvec4 vec) {
        int location = get_uniform_location(uniform_name);
        set_uniform_uint_vec4(location, vec);
    }

    void Shader::set_uniform_matrix_2f(GLint location, glm::mat2 mat, bool transpose) {
        use();
        glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_2f(const char* uniform_name, glm::mat2 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_2f(location, mat, transpose);
    }

    void Shader::set_uniform_matrix_3f(GLint location, glm::mat3 mat, bool transpose) {
        use();
        glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_3f(const char* uniform_name, glm::mat3 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_3f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_4f(GLint location, glm::mat4 mat, bool transpose) {
        use();
        glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_4f(const char* uniform_name, glm::mat4 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_4f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_2x3f(GLint location, glm::mat2x3 mat, bool transpose) {
        use();
        glUniformMatrix2x3fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_2x3f(const char* uniform_name, glm::mat2x3 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_2x3f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_3x2f(GLint location, glm::mat3x2 mat, bool transpose) {
        use();
        glUniformMatrix3x2fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_3x2f(const char* uniform_name, glm::mat3x2 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_3x2f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_2x4f(GLint location, glm::mat2x4 mat, bool transpose) {
        use();
        glUniformMatrix2x4fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_2x4f(const char* uniform_name, glm::mat2x4 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_2x4f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_4x2f(GLint location, glm::mat4x2 mat, bool transpose) {
        use();
        glUniformMatrix4x2fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_4x2f(const char* uniform_name, glm::mat4x2 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_4x2f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_3x4f(GLint location, glm::mat2x4 mat, bool transpose) {
        use();
        glUniformMatrix3x4fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_3x4f(const char* uniform_name, glm::mat3x4 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_3x4f(location, mat, transpose);
    }
    
    void Shader::set_uniform_matrix_4x3f(GLint location, glm::mat4x3 mat, bool transpose) {
        use();
        glUniformMatrix4x3fv(location, 1, transpose, glm::value_ptr(mat));
    }

    void Shader::set_uniform_matrix_4x3f(const char* uniform_name, glm::mat4x3 mat, bool transpose) {
        int location = get_uniform_location(uniform_name);
        set_uniform_matrix_4x3f(location, mat, transpose);
    }
}