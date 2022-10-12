#pragma once

#include "mesh.h"
#include "simpleengine/gfx/texture.h"
#include "../vector.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

//#include <assimp/mesh.h>

namespace simpleengine::gfx {
    enum ModelProcessingFlags : uint8_t {
        MdlProcFlag_NONE = 0b00000000,
        MdlProcFlag_FLIP_TEX_COORDS_VERTICALLY = 0b00000001,
        MdlProcFlag_FLIP_TEX_COORDS_HORIZONTALLY = 0b00000010,
        MdlProcFlag_CALCULATE_TANGENT_SPACE = 0b00000100,
        MdlProcFlag_TRANSPARENT = 0b00001000,
    };

    /**
     * @brief A Model is a group of Meshes read from the 3D model file.
     *
     * The engine uses assimp, so all formats that it supports can be found here:
     * https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
     * 
     */
    class Model : public simpleengine::Transformable {
    protected:
        std::string model_directory; // May be needed
        int additional_assimp_flags;
        int model_processing_flags;
    public:
        std::vector<gfx::Mesh> meshes;

        Model(std::string file_path, int model_processing_flags = ModelProcessingFlags::MdlProcFlag_NONE, int assimp_flags = 0);
    
        void load_model(std::string path);
        void process_node(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiNode* node, const aiScene* scene);
        gfx::Mesh process_mesh(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMesh* mesh, const aiScene* scene);

        std::unordered_map<aiTextureType, std::vector<Texture>> load_all_textures(aiMaterial* material);
        std::vector<std::shared_ptr<Texture>> load_material_texture(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMaterial* material, aiTextureType type);
    protected:
        void post_process();
    public:

        void vertically_flip_tex_coords();
        void horizontally_flip_tex_coords();
        void calculate_tangents();
    };
}