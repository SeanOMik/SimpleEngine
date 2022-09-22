#pragma once

#include "texture.h"

#include <assimp/material.h>
#include <unordered_map>

namespace simpleengine::gfx {
    class Material {
    public:
        std::unordered_map<aiTextureType, std::vector<Texture>> textures;

        float ambient_scalar;
        float diffuse_scalar;
        float specular_scalar;
        float shine;
        float reflectivity;

        Material(std::unordered_map<aiTextureType, std::vector<Texture>> textures, float shine = 1.f, float reflectivity = 0.f, float specular_scalar = 0.f, float ambient_scalar = 0.f, float diffuse_scalar = 0.f) :
                textures(textures), ambient_scalar(ambient_scalar), diffuse_scalar(diffuse_scalar), specular_scalar(specular_scalar),
                shine(shine), reflectivity(reflectivity) {

        }
    };
}