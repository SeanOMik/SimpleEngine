#pragma once

#include "texture.h"

#include <GLFW/glfw3.h>

namespace simpleengine::gfx {
    class Material {
    public:
        Texture texture;

        float ambient_scalar;
        float diffuse_scalar;
        float specular_scalar;
        float shine;
        float reflectivity;

        Material(Texture texture, float shine, float reflectivity, float specular_scalar, float ambient_scalar, float diffuse_scalar) :
                texture(texture), ambient_scalar(ambient_scalar), diffuse_scalar(diffuse_scalar), specular_scalar(specular_scalar),
                shine(shine), reflectivity(reflectivity) {

        }
    };
}