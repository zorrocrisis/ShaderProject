#include "HUD.h"

HUD::HUD() {}

HUD::~HUD() { delete textShader; delete overlayShader; }

void HUD::init(mgl::ShaderProgram* _textShader, mgl::ShaderProgram* _overlayShader, const std::string cuniform, const std::string puniform) {

    load();
    generateCharacters();

    // Enable blend (this way, we can make text transparent)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Reserve memory for text quads and transparent overlay
    configureQuads();

    // Assign text shader and uniforms used to render
    textShader = _textShader;
    overlayShader = _overlayShader;
    colorUniform = cuniform;
    orthoProjectionUniform = puniform;
}

void HUD::load() {

    // Initialize FreeType library
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "FREETYPE: Could not initialize FreeType Library" << std::endl;
    }

    // Load font (face) - default is arial.ttf
    if (FT_New_Face(ft, "../assets/fonts/arial.ttf", 0, &face))
    {
        std::cout << "FREETYPE: Failed to load font" << std::endl;
    }

}

void HUD::generateCharacters() {

    // Set fonts width and height. 0 makes dynamic width
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // We'll load 128 characters for future use
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph belonging to the created face
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture for this character
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // 2D texture, 1 channel (GL_RED), width/height of bitmap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character with all related information for later use (map)
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(std::pair<char, Character>(c, character));
    }

    // Free up font and library
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void HUD::configureQuads() {

    // Reserve memory so we can later render the character textures on quads
    glGenVertexArrays(1, &vaoText);
    glGenBuffers(1, &vboText);

    glBindVertexArray(vaoText); // binds
    glBindBuffer(GL_ARRAY_BUFFER, vboText);

    // Each 2D quad needs 6 vertices with 4 floats each
    // GL_DYNAMIC_DRAW since we'll update the memory regularly
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinds
    glBindVertexArray(0);

    // Reserve memory so we can later render a transparent overlay
    glGenVertexArrays(1, &vaoOverlay);
    glGenBuffers(1, &vboOverlay);

    glBindVertexArray(vaoOverlay); // binds
    glBindBuffer(GL_ARRAY_BUFFER, vboOverlay);

    // Each 2D quad needs 6 vertices with 4 floats each
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinds
    glBindVertexArray(0);
}

void HUD::checkViewportDimensions()
{
    // Check viewport properties and get resulting width and height (considering origin of viewport - indices 0 and 1)
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    viewportWidth = static_cast<float>(viewport[2] + viewport[0]);
    viewportHeight = static_cast<float>(viewport[3] + viewport[1]);

    // Also, calculate corresponding projection
    projection = glm::ortho(0.0f, static_cast<float>(viewport[2]), 0.0f, static_cast<float>(viewport[3]));

}

// Text body, position (in percentage of viewport), scale, color
void HUD::renderText(std::string text, glm::vec2 percentagePos, float scale, glm::vec3 color) {

    checkViewportDimensions();

    glm::vec2 pos = glm::vec2(viewportWidth * percentagePos.x, viewportHeight * percentagePos.y);

    // Bind text shader
    textShader->bind();

    // Update text colour and orthographic projection matrix
    glUniform3f((textShader->Uniforms[colorUniform].index), color.x, color.y, color.z);
    glUniformMatrix4fv(textShader->Uniforms[orthoProjectionUniform].index, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate text texture
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vaoText);

    // Iterate through all characters of the received text, reshape quads and apply corresponding textures
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        // Recover store info for each character
        Character ch = Characters[*c];

        // Position of quad, considering spacings and etc.
        float xpos = pos.x + ch.Bearing.x * scale;
        float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

        // Width and height of quad
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Update quads vertices for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // Render texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory (vertices)
        glBindBuffer(GL_ARRAY_BUFFER, vboText);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph/letter (note that advance is number of 1/64 pixels)
        pos.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    // Unbinds
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    textShader->unbind();
}

// Bottom-left corner position (in percentage), width, height and color of overlay
void HUD::renderOverlay(glm::vec2 percentagePos, float overlayW, float overlayH, glm::vec3 color) {

    // Bind overlay shader
    overlayShader->bind();

    checkViewportDimensions();

    glm::vec2 pos = glm::vec2(viewportWidth * percentagePos.x, viewportHeight * percentagePos.y);

    // Update overlay colour and orthographic projection matrix
    glUniform3f((overlayShader->Uniforms[colorUniform].index), color.x, color.y, color.z);
    glUniformMatrix4fv(overlayShader->Uniforms[orthoProjectionUniform].index, 1, GL_FALSE, glm::value_ptr(projection));

    // Bind
    glBindVertexArray(vaoOverlay);

    // Update quads vertices for each character
    float vertices[6][4] = {
       { pos.x,             pos.y + overlayH,   0.0f, 0.0f },
       { pos.x,             pos.y,              0.0f, 1.0f },
       { pos.x + overlayW,  pos.y,              1.0f, 1.0f },

       { pos.x,             pos.y + overlayH,   0.0f, 0.0f },
       { pos.x + overlayW,  pos.y,              1.0f, 1.0f },
       { pos.x + overlayW,  pos.y + overlayH,   1.0f, 0.0f }
    };

    // Update content of VBO memory (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vboOverlay);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbinds
    glBindVertexArray(0);
    overlayShader->unbind();
}