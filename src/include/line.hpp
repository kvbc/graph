/*
 *
 * Line
 *
 */

#ifndef LINE_H
#define LINE_H

#include "object.hpp"


class Line : public Object
{
public:
    Line(Shader& _shader,
         const Camera& _camera,
         float x1, float y1,
         float x2, float y2,
         GLuint _VBO = 0,
         GLuint _EBO = 0,
         Container *_container = nullptr)
        : Object(_shader, _camera, 4, 0, _VBO, _EBO, _container)
    {
        Container* container = getContainer();
        GLfloat* const vertices = container->getVertices();

        vertices[0] = x1;
        vertices[1] = y1;

        vertices[2] = x2;
        vertices[3] = y2;

        container->update_VAO();
    }
};


#endif /* LINE_H */
