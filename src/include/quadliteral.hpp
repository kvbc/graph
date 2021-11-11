/*
 *
 * Quad
 *
 */

#ifndef QUADLITERAL_H
#define QUADLITERAL_H

#include <array>
#include "object.hpp"


class Quadliteral : public Object
{
public:
    Quadliteral(Shader& _shader,
                const Camera& _camera,
                float x1, float y1,
                float x2, float y2,
                float x3, float y3,
                float x4, float y4,
                GLuint _VBO = 0,
                GLuint _EBO = 0,
                Container* _container = nullptr)
        : Object(_shader, _camera, 8, 6, _VBO, _EBO, _container)
    {
        Container* container = getContainer();
        GLuint* const indices = container->getIndices();
        GLfloat* const vertices = container->getVertices();

        vertices[0] = x1;
        vertices[1] = y1;

        vertices[2] = x2;
        vertices[3] = y2;

        vertices[4] = x3;
        vertices[5] = y3;

        vertices[6] = x4;
        vertices[7] = y4;

        // first triangle
        indices[1] = 1;
        indices[2] = 3;
        indices[0] = 0;

        // second triangle
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        container->update_VAO();
    }
};


#endif /* QUADLITERAL_H */
