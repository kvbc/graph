/*
 *
 * Object
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "container.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include <iostream>

class Object
{
public:
    glm::vec2 size = {1.0f, 1.0f};
    glm::vec2 position = {0.0f, 0.0f};

    Shader& shader;
    const Camera& camera;

    Object(Shader& _shader,
           const Camera& _camera,
           GLsizei _verticesCount,
           GLsizei _indicesCount = 0,
           GLuint _VBO = 0,
           GLuint _EBO = 0,
           Container* _container = nullptr);

    void destroy ();
    void updateModel ();
    void drawArrays (GLenum mode, GLsizei count) const;
    void drawElements (GLenum mode) const;

    /*
     *
     * Getters
     *
     */

    inline Container* getContainer () const;

private:
    glm::mat4 model;
    Container* container;
    bool container_allocated = false;

};


/*
 *
 * Getters
 *
 */

inline Container* Object::getContainer () const
{
    return container;
}


#endif /* OBJECT_H */
