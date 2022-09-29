#include "gfx/model.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "vector.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <optional>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace simpleengine::gfx {
    Model::Model(std::string file_path, int model_processing_flags, int assimp_flags): model_processing_flags(model_processing_flags),
            additional_assimp_flags(assimp_flags) {
        load_model(file_path);
    }

    void Model::post_process() {
        if (model_processing_flags & ModelProcessingFlags::MdlProcFlag_FLIP_TEX_COORDS_VERTICALLY) {
            vertically_flip_tex_coords();
        }

        if (model_processing_flags & ModelProcessingFlags::MdlProcFlag_FLIP_TEX_COORDS_HORIZONTALLY) {
            horizontally_flip_tex_coords();
        }
    }

    void Model::load_model(std::string path) {
        Assimp::Importer importer;

        // assimp post processing options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        const aiScene *scene = importer.ReadFile(path, additional_assimp_flags | aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        model_directory = path.substr(0, path.find_last_of('/'));

        std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>> processed_textures;
        process_node(processed_textures, scene->mRootNode, scene);

        post_process();
    }

    void Model::process_node(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back(process_mesh(processed_textures, mesh, scene));
        }

        // then do the same for each of its children
        for (int i = 0; i < node->mNumChildren; i++) {
            process_node(processed_textures, node->mChildren[i], scene);
        }
    }

    gfx::Mesh Model::process_mesh(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMesh* mesh, const aiScene* scene) {
        std::vector<LitVertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            LitVertex vertex;
            vertex.color = glm::vec3(1.f);

            simpleengine::Vectorf position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.position = position;

            // Only process normals if they exist.
            if (mesh->HasNormals()) {
                glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                vertex.normal = normal;
            }

            // Only process texture coords if they exist.
            if (mesh->HasTextureCoords(0)) {
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

        // Create a default material and white texture.
        auto white_texture = gfx::Texture::white_texture();
        std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>> default_textures;
        default_textures.emplace(white_texture.type, std::vector<std::shared_ptr<Texture>>{ std::make_shared<Texture>(white_texture) });
        gfx::Material mat(default_textures);

        if (mesh->mMaterialIndex >= 0) {
            std::cout << "TODO: Process model materials!" << std::endl;

            std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>> textures;
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            // Load Diffuse texture maps
            aiTextureType loading_type = aiTextureType_DIFFUSE;
            std::vector<std::shared_ptr<Texture>> diffuse_maps = load_material_texture(processed_textures, material, loading_type, TextureFlags::TexFlags_RGBA);
            if (!diffuse_maps.empty()) textures.emplace(loading_type, diffuse_maps);

            // Load specular texture maps
            loading_type = aiTextureType_SPECULAR;
            std::vector<std::shared_ptr<Texture>> spec_maps = load_material_texture(processed_textures, material, loading_type, TextureFlags::TexFlags_NO_COLOR);
            if (!spec_maps.empty()) textures.emplace(loading_type, spec_maps);

            // Load normals texture maps
            loading_type = aiTextureType_NORMALS;
            std::vector<std::shared_ptr<Texture>> normal_maps = load_material_texture(processed_textures, material, loading_type, TextureFlags::TexFlags_NO_COLOR);
            if (!normal_maps.empty()) textures.emplace(loading_type, normal_maps);

            // TODO Handle other types of texture maps

            if (!textures.empty()) {
                // TODO: Find a way to let the user set the scalars.
                mat = Material(textures);

                // Add `textures` into the `processed_textures` list.
                for (const auto& pair : textures) {
                    for (const auto& texture : pair.second) {
                        bool contains = false;

                        auto found = processed_textures.find(pair.first);
                        if (found != processed_textures.end()) {
                            for (const auto& processed_text : found->second) {
                                if (processed_text->path == texture->path) {
                                    contains = true;
                                    break;
                                }
                            }

                            if (!contains) {
                                //found->second
                                found->second.emplace_back(texture);
                            }
                        } else {
                            processed_textures.emplace(pair.first, std::vector<std::shared_ptr<Texture>>{ pair.second });
                        }
                    }
                }
            }
        }

        Mesh se_mesh(vertices, indices, mat);

        if (!mesh->HasNormals()) {
            se_mesh.calculate_normals();
        }

        return se_mesh;
    }

    std::unordered_map<aiTextureType, std::vector<Texture>> load_all_textures(aiMaterial* material) {
        // Load Diffuse texture maps
        return {};
    }

    std::vector<std::shared_ptr<Texture>> Model::load_material_texture(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>>& processed_textures, aiMaterial* material, aiTextureType type, TextureFlags texture_color) {
        std::vector<std::shared_ptr<Texture>> textures;

        for (int i = 0; i < material->GetTextureCount(type); i++) {
            aiString texture_path_ai;
            material->GetTexture(type, i, &texture_path_ai);
            std::string texture_path = texture_path_ai.C_Str();

            // If the texture has been read, we should skip it.
            bool skip = false;
            for (const auto& pair : processed_textures) {
                if (pair.first == type) {
                    for (const auto& texture : pair.second) {
                        if (texture->path == texture_path) {
                            textures.emplace_back(texture); // Push a copy of the texture for this Mesh.
                            skip = true;
                            break;
                        }
                    }
                }
            }
            if (skip) continue;

            std::stringstream ss;
            ss << model_directory << "/" << texture_path;
            std::string full_path = ss.str();

            Texture texture(full_path.c_str(), type, Texture::default_flags_no_color | texture_color);
            texture.path = texture_path;
            textures.emplace_back(std::make_shared<Texture>(texture));

            std::cout << "Texture full path: " << full_path << ", texture_path: " << texture_path << std::endl;
        }

        return textures;
    }

    void Model::vertically_flip_tex_coords() {
        for (auto& mesh : meshes) {
            for (auto& vertex : mesh.vertices) {
                vertex.tex_coord.y *= -1;
            }
        }
    }

    void Model::horizontally_flip_tex_coords() {
        for (auto& mesh : meshes) {
            for (auto& vertex : mesh.vertices) {
                vertex.tex_coord.x *= -1;
            }
        }
    }
} // namespace simpleengine::gfx