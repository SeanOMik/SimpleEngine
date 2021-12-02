#pragma once

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
        //glm::vec3 position;
        simpleengine::Vectorf position;
        glm::vec3 color;
        glm::vec2 tex_coord;
    };
}