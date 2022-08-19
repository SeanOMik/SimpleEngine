#include "objects/3d/mesh.h"

namespace simpleengine::objects_3d {
    std::vector<std::string> Mesh::split_string(std::string str, const char delim) {
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

    void Mesh::process_vertex(const std::vector<std::string>& vertex_data, const std::vector<glm::vec2>& in_textures,
            const std::vector<glm::vec3>& in_normals, std::vector<GLuint>& out_indicies, 
            std::vector<glm::vec2>& out_textures, std::vector<glm::vec3>& out_normals) {
        
        // Get the index the current vertex and put it in indicies
        int currentVertexIndex = stoi(vertex_data[0]) - 1;
        out_indicies.push_back(currentVertexIndex);

        // Read texture coords
        glm::vec2 current_tex = in_textures.at(stoi(vertex_data[1]) - 1);
        current_tex.y = 1 - current_tex.y;
        out_textures.at(currentVertexIndex) = current_tex;

        // Read normals
        glm::vec3 current_norm = in_normals.at(stoi(vertex_data[2]) - 1);
        out_normals.at(currentVertexIndex) = current_norm;
    }

    Mesh::Mesh(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::string filename) :
            Mesh(window, shader, texture, std::ifstream(filename, std::ios::in | std::ios::binary)) {

    }

    Mesh::Mesh(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::ifstream file_stream) :
            simpleengine::gfx::TexturedModel(window, shader, std::vector<gfx::Texture>{texture}, std::vector<LitVertex>()) {
        
        if (!file_stream.is_open()) {
            std::cerr << "File stream that was given to ObjModel::ObjModel is not open!" << std::endl;
            throw std::runtime_error("Failed to open ObjModel model file");
        }

        // The vertices, texture coords, and normals that were read from the obj file
        // these are not in a particular order.
        std::vector<glm::vec3> obj_vertices;
        std::vector<glm::vec2> obj_textures;
        std::vector<glm::vec3> obj_normals;

        // The texture coords and normals that have been sorted.
        std::vector<glm::vec2> textures;
        std::vector<glm::vec3> normals;

        // Read the vertices, texture coords, and normals. Break when run into indices
        std::string line;
        while (std::getline(file_stream, line)) {
            std::vector<std::string> line_tokens = split_string(line, ' ');

            if (line_tokens.front() == "v") {
                //glm::vec3 vertex(stof(line_tokens[1]), stof(line_tokens[2]), stof(line_tokens[3]));
                obj_vertices.emplace_back(stof(line_tokens[1]), stof(line_tokens[2]), stof(line_tokens[3]));
            } else if (line_tokens.front() == "vt") {
                obj_textures.emplace_back(stof(line_tokens[1]), stof(line_tokens[2]));
            } else if (line_tokens.front() == "vn") {
                obj_normals.emplace_back(stof(line_tokens[1]), stof(line_tokens[2]), stof(line_tokens[3]));
            } else if (line_tokens.front() == "f") {
                auto size = obj_vertices.size();
                textures.resize(size);
                normals.resize(size);
                break;
            }
        }

        // Process the indicies. This will sort everything for storing inside of the Vertex list.
        do {
            if (!line.starts_with("f")) {
                continue;
            }
            std::vector<std::string> line_tokens = split_string(line, ' ');
            std::vector<std::string> vertex1 = split_string(line_tokens[1], '/');
            std::vector<std::string> vertex2 = split_string(line_tokens[2], '/');
            std::vector<std::string> vertex3 = split_string(line_tokens[3], '/');
            
            process_vertex(vertex1, obj_textures, obj_normals, indicies, textures, normals);
            process_vertex(vertex2, obj_textures, obj_normals, indicies, textures, normals);
            process_vertex(vertex3, obj_textures, obj_normals, indicies, textures, normals);
        } while (std::getline(file_stream, line));
        
        file_stream.close();

        const int texture_id = 0;

        std::cout << "Texture ID: " << texture_id << std::endl;

        // Insert everything into lit_vertices.
        for (int i = 0; i < obj_vertices.size(); i++) {
            lit_vertices.emplace_back(simpleengine::Vectorf(obj_vertices.at(i)), glm::vec3(1.f), textures.at(i), normals.at(i), texture_id);
        }

        // Create VAO and EBO and assign buffers
        vao.bind();
        vbo.buffer(lit_vertices.data(), 0, sizeof(LitVertex) * lit_vertices.size());
        ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));

        // Enable VAO attributes
        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
        vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));
        vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
        vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Mesh::update(const float& delta_time) {
        this->rotate_y(0.0005f); // Slowly rotate (for debugging)
    }
}