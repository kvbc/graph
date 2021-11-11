/*
 *
 * Text Renderer
 * 
 * The code was by learnopengl.com
 * https://learnopengl.com/In-Practice/Text-Rendering
 * 
 * The whole TextRenderer and Font class shall be redesigned.
 * Why would the font be loaded twice, when all we change is the shader
 *
 */

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../shader.hpp"
#include "font.hpp"

#include <imgui.h>

class TextRenderer
{
public:
    TextRenderer(Shader& _shader);

    void done ();
    GLuint loadFont (const std::string& fontpath);
    glm::vec2 fontTextSize (GLuint _fontID, const std::string& text, float scaleX, float scaleY) const;

    void render (GLuint _fontID, const std::string& text, float x, float y, float scaleX, float scaleY, float colorR, float colorG, float colorB, float alpha) const;
    inline void render (GLuint _fontID, const std::string& text, float x, float y, float scaleX, float scaleY, const ImVec4& color) const;
    inline void render (GLuint _fontID, const std::string& text, float x, float y, float scaleX, float scaleY, const glm::vec4& color) const;

    inline bool anyError () const;

private:
    FT_Library ft;
    Shader& shader;

    GLuint fontID = 0;
    bool anyerr = false;
    std::vector<Font> fonts;
};

inline bool TextRenderer::anyError () const { return anyerr; }

inline void TextRenderer::render (GLuint _fontID, const std::string& text, float x, float y, float scaleX, float scaleY, const ImVec4& color)    const { render(_fontID, text, x, y, scaleX, scaleY, color.x, color.y, color.z, color.w); }
inline void TextRenderer::render (GLuint _fontID, const std::string& text, float x, float y, float scaleX, float scaleY, const glm::vec4& color) const { render(_fontID, text, x, y, scaleX, scaleY, color.x, color.y, color.z, color.w); }

#endif /* TEXTRENDERER_H */
