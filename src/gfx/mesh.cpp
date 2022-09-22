#include "gfx/mesh.h"
#include <optional>

namespace simpleengine::gfx {
    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material) :
            material(std::make_optional(material)), vertices(vertices), indicies(indicies),
           vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), ebo(gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false)),
           vao(gfx::VAO::init()) {

    }

    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, std::optional<Material> material) :
            material(material), vertices(vertices), indicies(indicies),
            vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), ebo(gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false)),
            vao(gfx::VAO::init()) {

    }

    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material, gfx::VBO ebo, gfx::VBO vbo, gfx::VAO vao) :
            vertices(vertices), indicies(indicies), material(material), ebo(ebo), vbo(vbo), vao(vao) {

    }

    std::vector<std::string> split_string(std::string str, const char delim) {
        std::istringstream ss(str);

        std::vector<std::string> tokens;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delim)) != std::string::npos) {
            token = str.substr(0, pos);
            tokens.push_back(token);
            str.erase(0, pos + 1);
        }
        tokens.push_back(str);

        return tokens;
    }

    void Mesh::destroy() {
        this->ebo.destroy();
        this->vbo.destroy();
        this->vao.destroy();
    }

    void Mesh::update(const float& delta_time) {
        this->rotate_y(1.f);
    }

    glm::vec3 Mesh::compute_face_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
        // Uses p2 as a new origin for p1,p3
        auto a = p3 - p2;
        auto b = p1 - p2;

        // Compute the cross product a X b to get the face normal
        return glm::normalize(glm::cross(a, b));
    }

    void Mesh::calculate_normals() {
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