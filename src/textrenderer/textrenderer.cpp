#include "../include/textrenderer/textrenderer.hpp"
#include <iostream>

TextRenderer::TextRenderer (Shader& _shader)
    : shader(_shader)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "Error: Failed to initalize the FreeType library" << std::endl;
        anyerr = true;
    }
}

void TextRenderer::done ()
{
    FT_Done_FreeType(ft);
}

GLuint TextRenderer::loadFont (const std::string &fontpath)
{
    fonts.push_back({ft, fontpath});
    return fontID++;
}

void TextRenderer::render
(
    GLuint _fontID,
    const std::string &text,
    float x, float y,
    float scaleX, float scaleY,
    float colorR, float colorG, float colorB, float alpha
) const
{
    shader.use();
    shader.setUniform("textColor", colorR, colorG, colorB, alpha);
    fonts[_fontID].render(text, x, y, scaleX, scaleY);
}

glm::vec2 TextRenderer::fontTextSize (GLuint _fontID, const std::string& text, float scaleX, float scaleY) const
{
    return fonts[_fontID].calcTextSize(text, scaleX, scaleY);
}
