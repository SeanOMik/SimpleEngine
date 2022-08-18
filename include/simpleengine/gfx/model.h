#pragma once

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "../vertex.h"
#include "../renderable.h"
#include "../transformable.h"

#include <vector>

namespace simpleengine::gfx {
    class Model : public simpleengine::Renderable, public simpleengine::Transformable {
    public:
        std::vector<LitVertex> vertices;
        std::vector<GLuint> indicies;
        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        gfx::Shader shader;

        Model(GLFWwindow* window, gfx::Shader shader, std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>());
        Model(GLFWwindow* window, GLuint shader_program, std::vector<LitVertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());
    protected:
        virtual void setup_vertices();
    public:
        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;

    private:
        glm::vec3 compute_face_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    public:
        /**
         * @brief Calculate the normals of the model.
         * 
         * @note This **will** overwrite the existing normals.
         *
         */
        void calculate_normals();
    };
}