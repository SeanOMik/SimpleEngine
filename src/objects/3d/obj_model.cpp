#include "objects/3d/obj_model.h""

namespace simpleengine::objects_3d {
    std::vector<std::string> ObjModel::split_string(std::string str, const char delim) {
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

    void ObjModel::process_vertex(const std::vector<std::string>& vertex_data, std::vector<GLuint>& indicies, const std::vector<glm::vec2>& in_textures,
            const std::vector<glm::vec3>& in_normals, std::vector<glm::vec2>& out_textures, std::vector<glm::vec3>& out_normals) {
        
        int currentVertexIndex = stoi(vertex_data[0]) - 1;
        indicies.push_back(currentVertexIndex);

        // Read texture coords
        glm::vec2 current_tex = in_textures.at(stoi(vertex_data[1]) - 1);
        current_tex.y = 1 - current_tex.y;
        out_textures.at(currentVertexIndex) = current_tex;

        // Read normals
        glm::vec3 current_norm = in_normals.at(stoi(vertex_data[2]) - 1);
        out_normals.at(currentVertexIndex) = current_norm;
    }

    ObjModel::ObjModel(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::string filename) :
            ObjModel(window, shader, texture, std::ifstream(filename, std::ios::in | std::ios::binary)) {

    }

    ObjModel::ObjModel(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::ifstream file_stream) :
            simpleengine::gfx::TexturedModel(window, shader, texture, std::vector<Vertex>()) {
        
        if (!file_stream.is_open()) {
            std::cerr << "File stream that was given to ObjModel::ObjModel is not open!" << std::endl;
            throw std::runtime_error("Failed to open ObjModel model file");
        }

        std::vector<glm::vec3> obj_vertices;
        std::vector<glm::vec2> obj_textures;
        std::vector<glm::vec3> obj_normals;

        std::vector<glm::vec2> textures;
        std::vector<glm::vec3> normals;

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

        do {
            if (!line.starts_with("f")) {
                continue;
            }
            std::vector<std::string> line_tokens = split_string(line, ' ');
            std::vector<std::string> vertex1 = split_string(line_tokens[1], '/');
            std::vector<std::string> vertex2 = split_string(line_tokens[2], '/');
            std::vector<std::string> vertex3 = split_string(line_tokens[3], '/');
            
            process_vertex(vertex1, indicies, obj_textures, obj_normals, textures, normals);
            process_vertex(vertex2, indicies, obj_textures, obj_normals, textures, normals);
            process_vertex(vertex3, indicies, obj_textures, obj_normals, textures, normals);
        } while (std::getline(file_stream, line));
        
        file_stream.close();

        for (int i = 0; i < obj_vertices.size(); i++) {
            lit_vertices.emplace_back(simpleengine::Vectorf(obj_vertices.at(i)), glm::vec3(1.f), textures.at(i), normals.at(i));
        }

        vao.bind();
        vbo.buffer(lit_vertices.data(), 0, sizeof(LitVertex) * lit_vertices.size());
        ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));

        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
        //vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
        vao.enable_attrib(vbo, 1, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
        vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ObjModel::update(const float& delta_time) {
        this->rotate_y(0.5f);
    }
}