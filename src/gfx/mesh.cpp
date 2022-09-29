#include "gfx/mesh.h"
#include "gfx/vbo.h"
#include "vector.h"
#include <optional>

namespace simpleengine::gfx {
    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material) :
            material(std::make_optional(material)), vertices(vertices), indicies(indicies),
           vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), ebo(gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false)),
           vao(gfx::VAO::init()), tangent_vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), bitangent_vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)) {

    }

    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, std::optional<Material> material) :
            material(material), vertices(vertices), indicies(indicies),
            vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), ebo(gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false)),
            vao(gfx::VAO::init()), tangent_vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)), bitangent_vbo(gfx::VBO::init(GL_ARRAY_BUFFER, false)) {

    }

    Mesh::Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material, gfx::VBO ebo, gfx::VBO vbo, gfx::VAO vao,
            gfx::VBO tangent_vbo, gfx::VBO bitangent_vbo) :
            vertices(vertices), indicies(indicies), material(material), ebo(ebo), vbo(vbo), vao(vao), tangent_vbo(tangent_vbo), bitangent_vbo(bitangent_vbo) {

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

    void Mesh::calculate_tangents() {
        // Resize the tangent and bitangents
        //tangents.clear();
        tangents.resize(vertices.size());
        //bitangents.clear();
        bitangents.resize(vertices.size());
        /* std::vector<simpleengine::Vectorf> tangents;
        std::vector<simpleengine::Vectorf> bitangents; */

        for (int i = 0; i < indicies.size(); i += 3) {
            int index0 = indicies[i];
            int index1 = indicies[i+1];
            int index2 = indicies[i+2];

            LitVertex& lit_vertex0 = vertices[index0];
            LitVertex& lit_vertex1 = vertices[index1];
            LitVertex& lit_vertex2 = vertices[index2];

            glm::vec3 pos0 = lit_vertex0.position;
            glm::vec3 pos1 = lit_vertex1.position;
            glm::vec3 pos2 = lit_vertex2.position;

            // Edges of the triangle : postion delta
            glm::vec3 delta_pos1 = pos1 - pos0;
            glm::vec3 delta_pos2 = pos2 - pos0;

            // UV delta
            glm::vec2 delta_uv1 = lit_vertex1.tex_coord - lit_vertex0.tex_coord;
            glm::vec2 delta_uv2 = lit_vertex2.tex_coord - lit_vertex0.tex_coord;

            float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
            glm::vec3 tangent = (delta_pos1 * delta_uv2.y   - delta_pos2 * delta_uv1.y)*r;
            glm::vec3 bitangent = (delta_pos2 * delta_uv1.x   - delta_pos1 * delta_uv2.x)*r;

            tangents[index0] = tangent;
            tangents[index1] = tangent;
            tangents[index2] = tangent;

            bitangents[index0] = bitangent;
            bitangents[index1] = bitangent;
            bitangents[index2] = bitangent;
        }

        /* this->tangents = std::optional<std::pair<std::vector<simpleengine::Vectorf>, std::vector<simpleengine::Vectorf>>>(
            {tangents, bitangents}
        ); */
    }
}