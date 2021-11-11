#include "include/container.hpp"
#include <iostream>
#include "include/debug/ClassManager.hpp"
#include <sstream>
#include <iomanip>

/*
 *
 * Container
 *
 */

Container::Container (GLuint VBO, GLuint EBO)
{
    m_VBO = VBO;
    m_EBO = EBO;
}

/*
 * New
 */

void Container::new_vertices (GLsizei count)
{
    delete[] vertices;
    vertices = new GLfloat[count];
    verticesCount = count;
}

void Container::new_indices (GLsizei count)
{
    delete[] indices;
    indices = new GLuint[count];
    indicesCount = count;
}

/*
 * Update
*/

void Container::update_VBO ()
{
    bind_VBO();
    if(verticesCount > VBOsize)
    {
        glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        VBOsize = verticesCount;
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesCount * sizeof(GLfloat), vertices);
    }
}

void Container::update_EBO ()
{
    bind_EBO();
    if(indicesCount > EBOsize)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
        EBOsize = indicesCount;
    }
    else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesCount * sizeof(GLuint), indices);
    }
}

void Container::update_VAO ()
{
    bind_VAO();
    update_VBO();

    if(m_EBO != 0)
    {
        update_EBO();
    }

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

/*
 *
 * ClassManager
 *
 */

void ClassManager::ImGui_printClassData (const char* nodelabel, const char* type, const Container* container)
{
    static const ImVec4 color = {1.0f, 0.0f, 1.0f, 1.0f};

    ImGui::PushID(container);

    if(ImGui_treeNode(nodelabel, type))
    {
        std::string str_VAO = std::to_string(container->get_VAO());
        std::string str_VBO = std::to_string(container->get_VBO());
        std::string str_EBO = std::to_string(container->get_EBO());
        std::string str_vertices = std::to_string(container->getVerticesCount()) + " [MAX " + std::to_string(container->getVBOSize()) + "]";
        std::string str_indices  = std::to_string(container->getIndicesCount())  + " [MAX " + std::to_string(container->getEBOSize()) + "]";

        ImGui_printLabel(color, "VAO", str_VAO.c_str());
        ImGui_printLabel(color, "VBO", str_VBO.c_str());
        ImGui_printLabel(color, "EBO", str_EBO.c_str());
        ImGui_printLabel(color, "vertices", str_vertices.c_str());
        ImGui_printLabel(color, "indices",  str_indices.c_str());

        std::stringstream stream;
        stream << std::hex << container->getVertices();

        ImGui_printLabel(color, "vertices[]", stream.str().c_str());

        const GLuint* indices = container->getIndices();
        if(indices == nullptr)
        {
            ImGui_printLabel(color, "indices[]", "NULL");
        }
        else
        {
            stream.str(std::string());
            stream.clear();
            stream << std::hex << indices;

            ImGui_printLabel(color, "indices[]", stream.str().c_str());
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}
