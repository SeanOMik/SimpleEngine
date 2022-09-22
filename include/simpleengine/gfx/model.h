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
        void process_node(aiNode* node, const aiScene* scene);
        gfx::Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> load_material_textures(aiMaterial* material, aiTextureType* type, std::string type_name);
    };
}