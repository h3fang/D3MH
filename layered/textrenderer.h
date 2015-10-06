#ifndef GL_TEXTRENDERER_H
#define GL_TEXTRENDERER_H

#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <GLSL/shader.h>

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    FT_Pos Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();

    void RenderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color, const glm::mat4 &mvp);

private:
    FT_Library ft;
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    Shader *shader;
};

#endif // GL_TEXTRENDERER_H
