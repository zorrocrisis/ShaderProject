#pragma once

#include <iostream>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <map>
#include <GL/glew.h>
#include "mglShader.hpp"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

class HUD
{
public: 
    unsigned int vaoText, vboText;
    unsigned int vaoOverlay, vboOverlay;
    FT_Library ft; // freetype library
    FT_Face face; // font

    // Text shader, color uniform and projection uniform
    mgl::ShaderProgram* textShader;
    mgl::ShaderProgram* overlayShader;
    std::string colorUniform;
    std::string orthoProjectionUniform;

    // Important variables for text projection (got from checkViewportDimensions)
    glm::mat4 projection; // for text projection
    float viewportWidth; // store current viewport width
    float viewportHeight; // store current viewport height

    HUD();
    ~HUD();

    void init(mgl::ShaderProgram* _textShader, mgl::ShaderProgram* _overlayShader, const std::string cuniform, const std::string puniform);
    void load();
    void generateCharacters();
    void configureQuads();
    void checkViewportDimensions();
    void renderText(std::string text, glm::vec2 percentagePos, float scale, glm::vec3 color);
    void renderOverlay(glm::vec2 percentagePos, float overlayW, float overlayH, glm::vec3 color);

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;


};

