#include "gfx/model.h"

namespace simpleengine::gfx {
    Model::Model(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, std::optional<Material> material) :
            material(material), vertices(vertices), indicies(indicies) {

    }

    void Model::update(const float& delta_time) {
        this->rotate_y(1.f);
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