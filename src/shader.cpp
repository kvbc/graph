#include "include/shader.hpp"
#include "include/debug/ClassManager.hpp"

#include <fstream>
#include <iostream>

void ClassManager::ImGui_printClassData (const char* nodelabel, const char* type, const Shader& shader)
{
    static const ImVec4 color = {0.0f, 1.0f, 1.0f, 1.0f};

    ImGui::PushID(&shader);

    if(ImGui_treeNode(nodelabel, type))
    {
        std::string str_id             = std::to_string(shader.getID());
        std::string str_vertexShader   = "\"" + shader.getVertexPath() + "\"";
        std::string str_fragmentShader = "\"" + shader.getFragmentPath() + "\"";
        std::string str_geometryShader = "\"" + shader.getGeometryPath() + "\"";

        ImGui_printLabel(color, "ID",              str_id.c_str());
        ImGui_printLabel(color, "vertex shader",   str_vertexShader.c_str());
        ImGui_printLabel(color, "fragment shader", str_fragmentShader.c_str());
        ImGui_printLabel(color, "geometry shader", str_geometryShader.c_str());

        ImGui::TreePop();
    }
    ImGui::PopID();
}

/*
 *
 * Overly complicated macros
 *
 */

#pragma region Uniform_Macros

#define SET_UNIFORM_PARAMS_1(TYPE) TYPE v0
#define SET_UNIFORM_PARAMS_2(TYPE) SET_UNIFORM_PARAMS_1(TYPE), TYPE v1
#define SET_UNIFORM_PARAMS_3(TYPE) SET_UNIFORM_PARAMS_2(TYPE), TYPE v2
#define SET_UNIFORM_PARAMS_4(TYPE) SET_UNIFORM_PARAMS_3(TYPE), TYPE v3

#define SET_UNIFORM_ARGS_1(TYPE) v0
#define SET_UNIFORM_ARGS_2(TYPE) SET_UNIFORM_ARGS_1(TYPE), v1
#define SET_UNIFORM_ARGS_3(TYPE) SET_UNIFORM_ARGS_2(TYPE), v2
#define SET_UNIFORM_ARGS_4(TYPE) SET_UNIFORM_ARGS_3(TYPE), v3

#define SET_UNIFORM(NUM, TYPE_LTR, TYPE)                                                            \
        void Shader::setUniform (const char* name, SET_UNIFORM_PARAMS_##NUM (TYPE) )                \
        {                                                                                           \
            glUniform ##NUM ##TYPE_LTR (getUniformLocation(name), SET_UNIFORM_ARGS_ ## NUM (TYPE)); \
        }

#define SET_UNIFORMV(NUM, TYPE_LTR, TYPE)                                                      \
        void Shader::setUniform ##NUM ##v (const char* name, GLsizei count, const TYPE *value) \
        {                                                                                      \
            glUniform ##NUM ##TYPE_LTR ##v (getUniformLocation(name), count, value);           \
        }

#define SET_UNIFORM_MATRIX(SUFFIX)                                                                                          \
        void Shader::setUniformMatrix ##SUFFIX (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value) \
        {                                                                                                                   \
            glUniformMatrix ##SUFFIX ##fv (getUniformLocation(name), count, transpose, value);                              \
        }

#define SET_UNIFORMS(TYPE_LTR, TYPE)        \
            SET_UNIFORM(1, TYPE_LTR, TYPE)  \
            SET_UNIFORM(2, TYPE_LTR, TYPE)  \
            SET_UNIFORM(3, TYPE_LTR, TYPE)  \
            SET_UNIFORM(4, TYPE_LTR, TYPE)

#define SET_UNIFORMVS(TYPE_LTR, TYPE)       \
            SET_UNIFORMV(1, TYPE_LTR, TYPE) \
            SET_UNIFORMV(2, TYPE_LTR, TYPE) \
            SET_UNIFORMV(3, TYPE_LTR, TYPE) \
            SET_UNIFORMV(4, TYPE_LTR, TYPE)

#pragma endregion

/*
 *
 * Helper Functions
 *
 */

static GLuint create_shader
(
    GLenum             shaderType,
    const std::string& shaderPath,
    GLint*             statusBuffer,
    GLchar*            infoLogBuffer,
    const std::string& errorPrefix
)
{
    std::ifstream ifs_shader(shaderPath);
    std::string shaderContent((std::istreambuf_iterator<char>(ifs_shader)),
                              (std::istreambuf_iterator<char>()         ));
    const char* const shaderCode = shaderContent.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, statusBuffer);
    if(!*statusBuffer)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLogBuffer);
        std::cout << errorPrefix << '\n' << infoLogBuffer << std::endl;
    }

    return shader;
}

/*
 *
 * Method definitions
 *
 */

GLint Shader::getUniformLocation (const char* name)
{
    auto uniformLocation = uniformLocations.find(name);
    if(uniformLocation == uniformLocations.end())
    {
        return (uniformLocations[name] = glGetUniformLocation(m_ID, name));
    }
    return uniformLocation->second;
}

Shader::Shader(const std::string& _vertexPath,
               const std::string& _fragmentPath,
               const std::string& _geometryPath)
{
    vertexPath = _vertexPath;
    fragmentPath = _fragmentPath;
    geometryPath = _geometryPath;

    GLint success;
    GLchar infoLog[512];

    GLuint vertexShader   = create_shader(GL_VERTEX_SHADER,   vertexPath,   &success, infoLog, "[Shader: Vertex Shader compilation failed]");
    GLuint fragmentShader = create_shader(GL_FRAGMENT_SHADER, fragmentPath, &success, infoLog, "[Shader: Fragment Shader compilation failed]");

    GLuint geometryShader = 0;
    if(!geometryPath.empty())
        geometryShader = create_shader(GL_GEOMETRY_SHADER, geometryPath, &success, infoLog, "[Shader: Geometry Shader compilation failed]");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    if(geometryShader)
        glAttachShader(m_ID, geometryShader);
    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cout << "Shader: Shader Program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::destroy () const
{
    glDeleteProgram(m_ID);
}

void Shader::use () const
{
    glUseProgram(m_ID);
}

SET_UNIFORMS(f, GLfloat)
SET_UNIFORMS(i, GLint)
SET_UNIFORMS(ui, GLuint)

SET_UNIFORMVS(f, GLfloat)
SET_UNIFORMVS(i, GLint)
SET_UNIFORMVS(ui, GLuint)

SET_UNIFORM_MATRIX(2)
SET_UNIFORM_MATRIX(3)
SET_UNIFORM_MATRIX(4)
SET_UNIFORM_MATRIX(2x3)
SET_UNIFORM_MATRIX(3x2)
SET_UNIFORM_MATRIX(2x4)
SET_UNIFORM_MATRIX(4x2)
SET_UNIFORM_MATRIX(3x4)
SET_UNIFORM_MATRIX(4x3)
