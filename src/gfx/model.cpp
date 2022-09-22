#include "gfx/model.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "vector.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <optional>

namespace simpleengine::gfx {
    Model::Model(std::string file_path) {
        load_model(file_path);
    }

    void Model::load_model(std::string path) {
        Assimp::Importer importer;

        // assimp post processing options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        model_directory = path.substr(0, path.find_last_of('/'));

        process_node(scene->mRootNode, scene);
    }

    void Model::process_node(aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back(process_mesh(mesh, scene));			
        }

        // then do the same for each of its children
        for (int i = 0; i < node->mNumChildren; i++) {
            process_node(node->mChildren[i], scene);
        }
    }

    gfx::Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<LitVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            LitVertex vertex;
            vertex.color = glm::vec3(1.f);
            vertex.texture_id = 0;

            simpleengine::Vectorf position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.position = position;

            glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.normal = normal;

            if(mesh->mTextureCoords[0]) {
                glm::vec2 tex_coord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

                vertex.tex_coord = tex_coord;
            }

            vertices.push_back(vertex);
        }

        // Process indices
        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];

            for (int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::optional<gfx::Material> op_mat;

        // TODO: Process material
        if(mesh->mMaterialIndex >= 0) {
            std::cout << "TODO: Process model materials!" << std::endl;

            /* aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = load_material_textures(material, 
                                                aiTextureType_DIFFUSE, "texture_diffuse");

            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            
            std::vector<Texture> specularMaps = load_material_textures(material, 
                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); */
        }

        // TODO: After we start processing materials, this can be put in the else statement as a fallback.

        auto white_texture = gfx::Texture::white_texture();
        //gfx::Material mat(white_texture);

        //gfx::Texture white_texture("examples/dev_testing/resources/white_texture.png");
        gfx::Material mat(white_texture, 1.f, 0.f, 0.f, 0.f, 0.f);

        op_mat = std::optional<gfx::Material>(mat);

        //return Mesh(vertices, indices, textures);
        return Mesh(vertices, indices, op_mat);
    }

    std::vector<Texture> Model::load_material_textures(aiMaterial* material, aiTextureType* type, std::string type_name) {

    }
} // namespace simpleengine::gfx