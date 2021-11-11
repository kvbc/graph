/*
 *
 * Shader
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <GL/gl.h>
#include <unordered_map>

class Shader
{
public:
    Shader (const std::string& _vertexPath,
            const std::string& _fragmentPath,
            const std::string& _geometryPath = "");

    void use() const;
    void destroy() const;

	/*
	 *
	 * Getters
	 *
	 */

    GLint getUniformLocation (const char* name);
    inline GLuint getID () const;
    inline const std::string& getVertexPath () const;
    inline const std::string& getFragmentPath () const;
    inline const std::string& getGeometryPath () const;


    /*
     *
     * Setters
     *
     */

    #pragma region Uniform_Setters

    void setUniform   (const char* name, GLfloat v0);
    void setUniform   (const char* name, GLfloat v0, GLfloat v1);
    void setUniform   (const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
    void setUniform   (const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    void setUniform   (const char* name, GLint v0);
    void setUniform   (const char* name, GLint v0, GLint v1);
    void setUniform   (const char* name, GLint v0, GLint v1, GLint v2);
    void setUniform   (const char* name, GLint v0, GLint v1, GLint v2, GLint v3);

    void setUniform   (const char* name, GLuint v0);
    void setUniform   (const char* name, GLuint v0, GLuint v1);
    void setUniform   (const char* name, GLuint v0, GLuint v1, GLuint v2);
    void setUniform   (const char* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

    void setUniform1v (const char* name, GLsizei count, const GLfloat *value);
    void setUniform2v (const char* name, GLsizei count, const GLfloat *value);
    void setUniform3v (const char* name, GLsizei count, const GLfloat *value);
    void setUniform4v (const char* name, GLsizei count, const GLfloat *value);

    void setUniform1v (const char* name, GLsizei count, const GLint *value);
    void setUniform2v (const char* name, GLsizei count, const GLint *value);
    void setUniform3v (const char* name, GLsizei count, const GLint *value);
    void setUniform4v (const char* name, GLsizei count, const GLint *value);

    void setUniform1v (const char* name, GLsizei count, const GLuint *value);
    void setUniform2v (const char* name, GLsizei count, const GLuint *value);
    void setUniform3v (const char* name, GLsizei count, const GLuint *value);
    void setUniform4v (const char* name, GLsizei count, const GLuint *value);

    void setUniformMatrix2   (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3   (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4   (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);

    void setUniformMatrix2x3 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3x2 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix2x4 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4x2 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3x4 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4x3 (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);

    #pragma endregion

private:
    GLuint m_ID;
	std::unordered_map<const char*, GLint> uniformLocations;

    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

inline 		 GLuint       Shader::getID              () const { return m_ID;         }
inline const std::string& Shader::getVertexPath      () const { return vertexPath;   }
inline const std::string& Shader::getFragmentPath    () const { return fragmentPath; }
inline const std::string& Shader::getGeometryPath    () const { return geometryPath; }


#endif /* SHADER_H */
