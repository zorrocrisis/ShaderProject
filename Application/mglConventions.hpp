////////////////////////////////////////////////////////////////////////////////
//
// Shader Naming and Other Useful Conventions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CONVENTIONS_HPP
#define MGL_CONVENTIONS_HPP

#include <string>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

// Uniforms
const std::string MODEL_MATRIX = "ModelMatrix";
const std::string VIEW_MATRIX = "ViewMatrix";
const std::string PROJECTION_MATRIX = "ProjectionMatrix";
const std::string TEXTURE_MATRIX = "TextureMatrix";
const std::string CAMERA_BLOCK = "Camera";
const std::string LIGHT_POSITION = "LightPosition";
const std::string NOISE_TEXTURE = "NoiseTexture";
const std::string COLOR_UNIFORM = "ColorUniform";
const std::string TEXTURE_REQUIREMENT = "NeedsNoise";
const std::string HIGHLIGHT_REQUIREMENT = "NeedsHighlight";
const std::string SILLHOUETTES_OFFSET = "SilhouetteOffset";
const std::string ORTHO_PROJECTION = "OrthoProjection"; // for text rendering
const std::string TEXT_TEXTURE = "textTex";
const std::string LEVERS = "Levers";

// Shader attributes
const std::string POSITION_ATTRIBUTE = "inPosition";
const std::string NORMAL_ATTRIBUTE = "inNormal";
const std::string TEXCOORD_ATTRIBUTE = "inTexcoord";
const std::string COLOR_ATTRIBUTE = "inColor";
const std::string VERTEX = "inVertex"; // for text rendering


// Selection conventions
const int BACKGROUND = -1; // background identification for selection
const int OVERLAY = -2;
const int LEFTMOUSE = 0;
const int RIGHTMOUSE = 1;
const int PRESS_DOWN = 1;

// Transformation conventions
const glm::vec3 NO_TRANSLATION = glm::vec3(.0f, .0f, .0f);
const glm::vec3 NO_SCALE = glm::vec3(1.f, 1.f, 1.f);
const glm::quat NO_ROTATION = { 1.f , 0.f , 0.f, 0.f };
const glm::vec3 Y_AXIS = glm::vec3(.0f, 1.0f, .0f);
const glm::vec3 X_AXIS = glm::vec3(1.0f, .0f, .0f);
const glm::vec3 Z_AXIS = glm::vec3(.0f, .0f, 1.0f);

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CONVENTIONS_HPP */
