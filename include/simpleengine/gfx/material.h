#pragma once

#include "rendering_type.h"
#include "texture.h"

#include <assimp/material.h>

#include <unordered_map>
#include <memory>

namespace simpleengine::gfx {
    class Material {
    public:
        RenderingType rendering_type;

        // TODO: Only one of each texture type.
        std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>> textures;

        float ambient_strength;
        float diffuse_strength;

        /**
         * @brief This strengthens the brightness of a specular highlight.
         * 
         */
        float specular_strength;
        
        /**
         * @brief The shininess value of the highlight. (Radius of specular highlight?)
         
         * The higher the shininess value of an object, the more it properly reflects the light
         * instead of scattering it all around and thus the smaller the highlight becomes.
         * 
         * The shader multiplies this by 32 to get the specular highlight.
         * 
         */
        float shine_factor;

        Material(std::unordered_map<aiTextureType, std::vector<std::shared_ptr<Texture>>> textures, RenderingType rendering_type, float shine = 1.f,
                float specular_scalar = 1.f, float ambient_scalar = 1.f, float diffuse_scalar = 1.f) : textures(textures), rendering_type(rendering_type),
                ambient_strength(ambient_scalar), diffuse_strength(diffuse_scalar), specular_strength(specular_scalar), shine_factor(shine) {

        }
    };
}