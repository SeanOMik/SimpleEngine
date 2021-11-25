//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_VERTEX_H
#define SIMPLEENGINE_VERTEX_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <glm/glm.hpp>

namespace simpleengine {
    class Vertex {
    public:
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 tex_coord;
    };
}

#endif //SIMPLEENGINE_VERTEX_H