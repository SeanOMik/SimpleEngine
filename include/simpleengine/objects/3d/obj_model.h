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
    class ObjModel : public simpleengine::gfx::TexturedModel {
    private:
        std::vector<std::string> split_string(std::string str, const char delim);
        static void process_vertex(const std::vector<std::string>& vertex_data, std::vector<GLuint>& indicies, const std::vector<glm::vec2>& in_textures,
                const std::vector<glm::vec3>& in_normals, std::vector<glm::vec2>& out_textures, std::vector<glm::vec3>& out_normals);
    private:
        /**
         * @brief This is replaced with `lit_vertices`!!!!
         * 
         */
        using simpleengine::gfx::Model::vertices;
    public:
        std::vector<LitVertex> lit_vertices;

        ObjModel(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::string filename);
        ObjModel(GLFWwindow *window, gfx::Shader shader, gfx::Texture texture, std::ifstream file_stream);

        virtual void update(const float& delta_time) override;

        /* virtual void render(GLFWwindow* target) override {
            shader.use();

            shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

            // When binding to the texture, also tell the shader if the texture is set or not.
            if (texture.has_value()) {
                shader.set_uniform_int("texture_is_set", true, false);
                texture.value().bind();
            } else {
                shader.set_uniform_int("texture_is_set", false, false);
            }

            vao.bind();
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        } */
    };
}