#pragma once

#include <functional>
#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#elif
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <glm/glm.hpp>

#include "vector.h"

namespace simpleengine {
    class Vertex {
    public:
        simpleengine::Vectorf position;
        glm::vec3 color;
        glm::vec2 tex_coord;

        Vertex() = default;

        Vertex(simpleengine::Vectorf position, glm::vec3 color, glm::vec2 tex_coord) : position(position), color(color), tex_coord(tex_coord) {
            
        }
    };

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

        LitVertex() = default;

        LitVertex(simpleengine::Vectorf position, glm::vec3 color, glm::vec2 tex_coord, glm::vec3 normal) :
                position(position), color(color), tex_coord(tex_coord), normal(normal) {
            
        }
    };
}