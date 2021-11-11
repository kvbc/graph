/*
 *
 * Container
 * VAO and Buffer Object managment
 *
 */

#ifndef CONTAINER_H
#define CONTAINER_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class Container
{
public:
    Container (GLuint VBO = 0,
               GLuint EBO = 0);

    void new_vertices (GLsizei count);
    void new_indices  (GLsizei count);

    void update_VAO ();
    void update_VBO ();
    void update_EBO ();

    /*
     *
     * Getters
     *
     */

    // ID
    inline GLuint get_VBO () const;
    inline GLuint get_VAO () const;
    inline GLuint get_EBO () const;

    // Buffer
    inline const GLfloat* const getVertices () const;
    inline const GLuint*  const getIndices  () const;
    inline GLfloat* const getVertices ();
    inline GLuint*  const getIndices  ();

    // Count
    inline GLsizei getVerticesCount () const;
    inline GLsizei getIndicesCount () const;

    // Buffer size
    inline GLsizei getVBOSize () const;
    inline GLsizei getEBOSize () const;

    /*
     *
     * Buffer modifiers
     *
     */

    // VBO
    inline void gen_VBO ();
    inline void bind_VBO () const;
    inline void del_VBO ();

    // EBO
    inline void gen_EBO ();
    inline void bind_EBO () const;
    inline void del_EBO ();

    // VAO
    inline void gen_VAO ();
    inline void del_VAO ();
    inline void bind_VAO () const;

private:
    // Buffer
    GLuint* indices = nullptr;
    GLfloat* vertices = nullptr;

    // Count
    GLsizei indicesCount = 0;
    GLsizei verticesCount = 0;

    // Buffer size
    GLsizei VBOsize = 0;
    GLsizei EBOsize = 0;

    // ID
    GLuint m_VBO = 0;
    GLuint m_VAO = 0;
    GLuint m_EBO = 0;
};

/*
 *
 * Getters
 *
 */

// ID
inline GLuint Container::get_VBO () const { return m_VBO; }
inline GLuint Container::get_VAO () const { return m_VAO; }
inline GLuint Container::get_EBO () const { return m_EBO; }

// Buffer
inline const GLuint*  const Container::getIndices  () const { return indices;  }
inline const GLfloat* const Container::getVertices () const { return vertices; }
inline GLuint*  const Container::getIndices  () { return indices;  }
inline GLfloat* const Container::getVertices () { return vertices; }

// Count
inline GLsizei Container::getIndicesCount  () const { return indicesCount;  }
inline GLsizei Container::getVerticesCount () const { return verticesCount; }

// Buffer size
inline GLsizei Container::getVBOSize () const { return VBOsize; }
inline GLsizei Container::getEBOSize () const { return EBOsize; }

/*
 *
 * Buffer modifiers
 *
 */

// VBO
inline void Container::gen_VBO  ()       { glGenBuffers   (1, &m_VBO); }
inline void Container::del_VBO  ()       { glDeleteBuffers(1, &m_VBO); }
inline void Container::bind_VBO () const { glBindBuffer(GL_ARRAY_BUFFER, m_VBO); }

// EBO
inline void Container::gen_EBO  ()       { glGenBuffers   (1, &m_EBO); }
inline void Container::del_EBO  ()       { glDeleteBuffers(1, &m_EBO); }
inline void Container::bind_EBO () const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); }

// VAO
inline void Container::gen_VAO  ()       { glGenVertexArrays   (1, &m_VAO); }
inline void Container::del_VAO  ()       { glDeleteVertexArrays(1, &m_VAO); }
inline void Container::bind_VAO () const { glBindVertexArray       (m_VAO); }


#endif /* CONTAINER_H */
