#include "node.h"

void node::linkChild(node* next_lvl) {
	next_level = next_lvl;
}
void node::updateModel() {
    glm::mat4 I(1.f);
    Model = glm::translate(Translation) * glm::toMat4(Rotation) * glm::scale(I,Scale) * I;
}

void node::translateModel(glm::vec3 translat) {
    Translation = translat;
    updateModel();
}

void node::rotateModel(glm::quat quat) {
    Rotation = quat;
    updateModel();
}

void node::initializeModels() {
    node* current;
    updateModel();
    current = next_level;
    int i = 1;
    while (current != NULL) {

        current->updateModel();
        i++;
        current = current->next_level;
    }
}

void node::addYRotation(float deltaAngle) {

    // Get equivalent quaternion and "add" it to our rotation component
    glm::quat yRotation = glm::angleAxis(deltaAngle, mgl::Y_AXIS);
    Rotation = yRotation * Rotation;
    updateModel();
}


void node::addXRotation(float deltaAngle) {

    // Increment yAngle with delta
    glm::quat xRotation = glm::angleAxis(deltaAngle, mgl::X_AXIS);
    Rotation = xRotation * Rotation;
    updateModel();
}

void node::addZRotation(float deltaAngle) {

    // Increment yAngle with delta
    glm::quat zRotation = glm::angleAxis(deltaAngle, mgl::Z_AXIS);
    Rotation = zRotation * Rotation;
    updateModel();
}


void node::drawCelSelf() {

    Shaders->bind();
    
    // Get locations of uniforms
    GLint colorLocation = Shaders->Uniforms[mgl::COLOR_UNIFORM].index;
    GLint needTextureLocation = Shaders->Uniforms[mgl::TEXTURE_REQUIREMENT].index;
    GLint highlightLocation = Shaders->Uniforms[mgl::HIGHLIGHT_REQUIREMENT].index;
    GLint silhouetteOffsetLocation = Shaders->Uniforms[mgl::SILLHOUETTES_OFFSET].index;

    // Well use the same model for both the silhouette and the main object
    glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(Model));

    /// Draw silhouette
    glCullFace(GL_FRONT); // front face culling 
    glUniform3f(colorLocation, 0.f, 0.f, 0.f); // all black
    glUniform1i(highlightLocation, 0); // no highlight
    glUniform1f(silhouetteOffsetLocation, 0.05f); // silhouette offset
    glUniform1i(needTextureLocation, 0); // silhouette doesnt need texture
    Mesh->draw();

    glCullFace(GL_BACK); // turn back face culling on again

    Shaders->unbind();

    Shaders->bind();

    // Update shaders if node needs texture
    if (NodeNeedsTexture)
        TextInfo->updateShader(Shaders);

    /// Draw "normal" model
    glUniform3f(colorLocation, Color[0], Color[1], Color[2]); // object colour
    glUniform1i(needTextureLocation, NodeNeedsTexture); // check if node needs to texture
    glUniform1i(highlightLocation, NodeNeedsHighlight); // check if node needs highlight/if object is selected
    glUniform1f(silhouetteOffsetLocation, 0.0f); // we don't need a silhouette

    Mesh->draw();

    Shaders->unbind();

}



