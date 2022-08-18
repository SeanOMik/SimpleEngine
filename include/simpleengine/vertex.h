#pragma once

#include <functional>
#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <glm/glm.hpp>

#include "vector.h"

namespace simpleengine {
    

    /**
     * @brief A `Vertex` that can be lit up.
     * 
     */
    class LitVertex {
    public:
        simpleengine::Vectorf position;
        glm::vec3 color;
        glm::vec2 tex_coord;
        glm::vec3 normal;
        float texture_id = -1.f;

        LitVertex() = default;

        LitVertex(simpleengine::Vectorf position, glm::vec3 color, glm::vec2 tex_coord, int texture_id = -1.f) :
                position(position), color(color), tex_coord(tex_coord), normal(glm::vec3(0.f)), texture_id((float) texture_id) {
            
        }

        LitVertex(simpleengine::Vectorf position, glm::vec3 color, glm::vec2 tex_coord, glm::vec3 normal, int texture_id = -1.f) :
                position(position), color(color), tex_coord(tex_coord), normal(normal), texture_id((float) texture_id) {
            
        }

        LitVertex(simpleengine::Vectorf position, glm::vec2 tex_coord, glm::vec3 normal, int texture_id = -1.f) :
                position(position), color(glm::vec3(1.f)), tex_coord(tex_coord), normal(normal), texture_id((float) texture_id) {
            
        }
    };
}