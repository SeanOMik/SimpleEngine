#pragma once

#include <gl/glew.h>
#include <gl/gl.h>

#include <glm/glm.hpp>

#include "vector.h"

namespace simpleengine {
    class Vertex {
    public:
        //glm::vec3 position;
        simpleengine::Vectorf position;
        glm::vec3 color;
        glm::vec2 tex_coord;
    };
}