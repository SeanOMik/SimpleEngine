#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../optional.h"
#include "../../vector.h"
#include "../../vertex.h"
#include "../../renderable.h"
#include "../../transformable.h"
#include "../../gfx/vao.h"
#include "../../gfx/vbo.h"
#include "../../gfx/shader.h"
#include "../../gfx/texture.h"
#include "../../gfx/textured_model.h"

namespace simpleengine::objects_3d {
    class Mesh : public simpleengine::gfx::TexturedModel {
    private:
        /**
         * @brief Split a string multiple times (if possible) with a character delimiter.
         * 
         * @param str The string to split.
         * @param delim The character to split by.
         * @return std::vector<std::string> The tokens that were split out of str.
         */
        std::vector<std::string> split_string(std::string str, const char delim);

        /**
         * @brief Process a vertex from tokens read from the .obj file.
         * 
         * @param vertex_data The vertex string tokens to process the vertex from.
         * @param in_textures The texture coords that are unsorted, just read from the .obj file.
         * @param in_normals The normals that are unsorted, just read from the .obj file.
         * @param out_indicies (out) The vector to insert the indicies that were extracted from `vertex_data` into.
         * @param out_textures (out) The vector to insert the texture coords that were extracted from `vertex_data` into.
         * @param out_normals (out) The vector to insert the normals that were extracted from `vertex_data` into.
         */
        static void process_vertex(const std::vector<std::string>& vertex_data, const std::vector<glm::vec2>& in_textures,
                const std::vector<glm::vec3>& in_normals, std::vector<GLuint>& out_indicies, std::vector<glm::vec2>& out_textures, std::vector<glm::vec3>& out_normals);
    private:
        /**
         * @brief This is replaced with `lit_vertices`!!!!
         * 
         */
        using simpleengine::gfx::Model::vertices;
    public:
        std::vector<LitVertex> lit_vertices;

        Mesh(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::string filename);
        Mesh(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::ifstream file_stream);

        virtual void update(const float& delta_time) override;
    };
}