////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace mgl {

class Camera;

///////////////////////////////////////////////////////////////////////// Camera

class Camera {
private:
  GLuint UboId;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  glm::vec3 CurrentEyePosition;

public:
  Camera(GLuint bindingpoint);
  ~Camera();
  glm::mat4 getViewMatrix();
  void setViewMatrix(const glm::mat4 &viewmatrix);
  glm::mat4 getProjectionMatrix();
  void setProjectionMatrix(const glm::mat4 &projectionmatrix);

  void storeEyePosition(const glm::vec3& eye);
  glm::vec3 getEyePosition();
  glm::vec3 getView();
  glm::vec3 getUp();
  glm::vec3 getRight();

};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CAMERA_HPP */
