#include "gfx/model.h"

namespace simpleengine::gfx {
    Model::Model(GLFWwindow* window, gfx::Shader shader, std::vector<LitVertex> vertices, std::vector<GLuint> indicies) :
            simpleengine::Renderable(window), shader(shader), vertices(vertices), indicies(indicies), vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)),
            ebo(gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false)), vao(gfx::VAO::init()) {

        //setup_vertices();
    }

    Model::Model(GLFWwindow* window, GLuint shader_program, std::vector<LitVertex> vertices, std::vector<GLuint> indicies) :
            Model(window, gfx::Shader(shader_program), vertices, indicies) {

    }

    void Model::setup_vertices() {
        vao.bind();
        vbo.buffer(vertices.data(), 0, sizeof(LitVertex) * vertices.size());
        if (!indicies.empty()) {
            ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));
        }

        // Enable VAO attributes
        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
        vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));
        // Attribute 2 is used for normals
        vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
        vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id));

        /* vao.disable_attrib(vbo, 2);
        vao.disable_attrib(vbo, 4);
        vao.set_attrib_value(vbo, 4, -1.f); */

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Model::update(const float& delta_time) {
        this->rotate_y(1.f);
    }

    void Model::render(GLFWwindow* target) {
        shader.use();
        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

        // When binding to the texture, also tell the shader if the texture is set or not.
        //shader.set_uniform_int("texture_is_set", (GLint) false, false);
        
        vao.bind();
        if (indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    }

    glm::vec3 Model::compute_face_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
        // Uses p2 as a new origin for p1,p3
        auto a = p3 - p2;
        auto b = p1 - p2;

        // Compute the cross product a X b to get the face normal
        return glm::normalize(glm::cross(a, b));
    }

    void Model::calculate_normals() {
        std::vector<glm::vec3> normals = std::vector<glm::vec3>(vertices.size());

        for (int i = 0; i < indicies.size(); i+=3) {
            const glm::vec3& a = vertices[indicies[i]].position;
            const glm::vec3& b = vertices[indicies[i + 1]].position;
            const glm::vec3& c = vertices[indicies[i + 2]].position;
            glm::vec3 normal = compute_face_normal(a, b, c);

            normals[indicies[i]] += normal;
            normals[indicies[i + 1]] += normal;
            normals[indicies[i + 2]] += normal;
        }

        for (int i = 0; i < normals.size(); i++) {
            normals[i] = glm::normalize(normals[i]);

            vertices[i].normal = normals[i];
        }
    }
}