#pragma once

#include "mesh.h"
#include "simpleengine/gfx/texture.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

//#include <assimp/mesh.h>

namespace simpleengine::gfx {
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
    public:
        std::vector<gfx::Mesh> meshes;

        Model(std::string file_path);
    
        void load_model(std::string path);
        void process_node(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiNode* node, const aiScene* scene);
        gfx::Mesh process_mesh(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMesh* mesh, const aiScene* scene);

        std::unordered_map<aiTextureType, std::vector<Texture>> load_all_textures(aiMaterial* material);
        std::vector<std::shared_ptr<Texture>> load_material_texture(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMaterial* material, aiTextureType type);
    };
}