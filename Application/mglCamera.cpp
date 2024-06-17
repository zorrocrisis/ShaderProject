#include <glm/gtc/type_ptr.hpp>

#include "mglCamera.hpp"

namespace mgl {

///////////////////////////////////////////////////////////////////////// Camera

Camera::Camera(GLuint bindingpoint)
    : ViewMatrix(glm::mat4(1.0f)), ProjectionMatrix(glm::mat4(1.0f)) {
  glGenBuffers(1, &UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingpoint, UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera() {
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glDeleteBuffers(1, &UboId);
}

glm::mat4 Camera::getViewMatrix() { return ViewMatrix; }

void Camera::setViewMatrix(const glm::mat4 &viewmatrix) {
  ViewMatrix = viewmatrix;
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(ViewMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glm::mat4 Camera::getProjectionMatrix() { return ProjectionMatrix; }

void Camera::setProjectionMatrix(const glm::mat4 &projectionmatrix) {
  ProjectionMatrix = projectionmatrix;
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(ProjectionMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::storeEyePosition(const glm::vec3& eye)
{
    CurrentEyePosition = eye;
}

glm::vec3 Camera::getEyePosition()
{
    return CurrentEyePosition;
}

glm::vec3 Camera::getView()
{
    return -glm::transpose(getViewMatrix())[2];
}

glm::vec3 Camera::getUp()
{
    return glm::transpose(getViewMatrix())[1];
}

glm::vec3 Camera::getRight()
{
    return glm::transpose(getViewMatrix())[0];
}


////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
