#include "include/object.hpp"
#include "include/debug/ClassManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

/*
 *
 * Object
 *
 */

Object::Object
(
    Shader& _shader,
    const Camera& _camera,
    GLsizei _verticesCount,
    GLsizei _indicesCount,
    GLuint _VBO,
    GLuint _EBO,
    Container* _container
)
    : shader(_shader), camera(_camera)
{
    if(_container != nullptr)
    {
        container = _container;
    }
    else
    {
        container_allocated = true;
        container = new Container(_VBO, _EBO);

        container->gen_VAO();

        if(_VBO == 0)      container->gen_VBO();
        if(_verticesCount) container->new_vertices(_verticesCount);

        if(_EBO == 0)     container->gen_EBO();
        if(_indicesCount) container->new_indices(_indicesCount);
    }


}

// Update the model matrix
void Object::updateModel ()
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, {position.x, position.y, 0.0f});
    model = glm::scale(model, {size.x, size.y, 0.0f});
}

void Object::destroy ()
{
    if(container_allocated)
    {
        container->del_VAO();

        container->del_VBO();
        delete[] container->getVertices();

        if(container->getIndices() != nullptr)
        {
            container->del_EBO();
            delete[] container->getIndices();
        }

        delete container;
    }
}

void Object::drawArrays (GLenum mode, GLsizei count) const
{
    shader.use();
    shader.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));

    container->bind_VAO();
    glDrawArrays(mode, 0, count);
}

void Object::drawElements (GLenum mode) const
{
    shader.use();
    shader.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
    
    container->bind_VAO();
    glDrawElements(mode, container->getIndicesCount(), GL_UNSIGNED_INT, 0);
}

/*
 *
 * ClassManager
 *
 */

void ClassManager::ImGui_printClassData (const char* nodelabel, const char* type, const Object& object)
{
    static const ImVec4 color = {0.0f, 1.0f, 0.0f, 1.0f};

    ImGui::PushID(&object);

    if(ImGui_treeNode(nodelabel, type))
    {
        std::string str_size     = std::to_string(object.size.x)     + ", " + std::to_string(object.size.y);
        std::string str_position = std::to_string(object.position.x) + ", " + std::to_string(object.position.y);

        ImGui_printClassData(object.getContainer());

        ImGui_printClassData(object.shader);
        ImGui_printClassData(object.camera);

        ImGui_printLabel(color, "size",     str_size.c_str());
        ImGui_printLabel(color, "position", str_position.c_str());

        ImGui::TreePop();
    }
    ImGui::PopID();
}
