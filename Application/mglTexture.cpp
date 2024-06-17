#include <cassert>
#include <sstream>

#include "mglTexture.hpp"
#include "stb_image.h"
#include <random>

namespace mgl {

//////////////////////////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
                         const std::string &_uniform, Texture *_texture,
                         Sampler *_sampler) {
  unit = _unit;
  uniform = _uniform;
  index = _index;
  texture = _texture;
  sampler = _sampler;
}

void TextureInfo::updateShader(ShaderProgram *shader) {
  glActiveTexture(unit);
  texture->bind();
  if (sampler)
    sampler->bind(index);
  glUniform1i(shader->Uniforms[uniform].index, unit);
}

//////////////////////////////////////////////////////////////////////// Texture

Texture::Texture() : id(-1) {}

Texture::~Texture() {}

////////////////////////////////////////////////////////////////////// Texture2D

void Texture3D::bind() { glBindTexture(GL_TEXTURE_3D, id); }

void Texture3D::unbind() { glBindTexture(GL_TEXTURE_3D, 0); }

void Texture3D::createWhiteNoise(int width, int height, int depth) {

    std::random_device rd; // Seed for the random number generator
    std::mt19937 gen(rd()); // Random number generator
    std::uniform_int_distribution<> dis(0, 255); // generates uniform int distribution (256 for colour)

    // Allocate space for image data (4 is the number of channels)
    unsigned char* imageData = new unsigned char[width * height * depth * 4];

    for (int i = 0; i < width * height * depth * 4; i += 4) {
        int noise = dis(gen); // Make R = G = B for greyscale
        imageData[i] = static_cast<unsigned char> (noise);
        imageData[i + 1] = static_cast<unsigned char> (noise);
        imageData[i + 2] = static_cast<unsigned char> (noise);
        imageData[i + 3] = static_cast < unsigned char> (255);
    }

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_3D, id);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, imageData);

  glGenerateMipmap(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D, 0);

  delete [] imageData;
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
