/*
 *
 * Font
 * 
 * The code was inspired by learnopengl.com
 * https://learnopengl.com/In-Practice/Text-Rendering
 * 
 * The whole TextRenderer and Font class shall be redesigned.
 * Why would the font be loaded twice, when all we change is the shader
 *
 */

#ifndef TEXTRENDERER_FONT_H
#define TEXTRENDERER_FONT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#define MIN_CHAR 32  // ' '
#define MAX_CHAR 126 // '~'

struct Character
{
    unsigned int texID;   // ID handle of the glyph texture
    glm::ivec2   size;    // size of glyph
    glm::ivec2   bearing; // offset from baseline to left/top of glyph
    unsigned int advance; // offset to advance to next glyph
};

class Font
{
public:
    Font() = default;
    Font(const FT_Library& ft, const std::string& fontpath);

    void render (const std::string& text, float x, float y, float scaleX, float scaleY) const;

    glm::vec2 calcTextSize (const std::string& text, float scaleX, float scaleY) const;

    inline bool anyError () const;

private:
    GLuint VBO;
    GLuint VAO;
    bool anyerr = false;
    Character characters[MAX_CHAR - MIN_CHAR];
};


inline bool Font::anyError () const
{
    return anyerr;
}


#endif /* TEXTRENDERER_FONT_H */
