#include "MouseSelection.h"

void MouseSelection::ConfigureStencilForSelection() {

    // Clear framebuffer and make sure stencil only contains zero
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Enable stencil test
    glEnable(GL_STENCIL_TEST);

    // Stencil buffer will be written to whenever colour buffer is
    // written to (only when depth test succeeds). When that happens,
    // replace the value with a fixed new value
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void MouseSelection::AssignReference(int i, std::string name) { // Should always be done before draw

    if (name == "background") {
        std::cout << "MouseSelection::AssignReference - Illegal name!" << std::endl;
        return;
    }

    // Set the stencil function to always pass the stencil test,
    // and set the reference/index value (-1 is already used for background)
    glStencilFunc(GL_ALWAYS, i + 1, -1);

    // Insert into map for future reference
    referenceCorresponce.insert({ i + 1, name });
}

std::string MouseSelection::RetrieveReference(int i) {

    // Get corresponding name from map
    if (i == -1)
        return "background"; // this could have been coded better...
    else
        return referenceCorresponce.find(i + 1)->second;

}

GLuint MouseSelection::ReadPixel(int mouseIdent) {

    double x;
    double y;
    GLuint index;

    // Checks if we want to read pixel for left or right mouse button
    if (mouseIdent == 0)
    {
        x = LeftMB.lastPositionX;
        y = LeftMB.lastPositionY;
    }
    else if (mouseIdent == 1)
    {
        x = RightMB.lastPositionX;
        y = RightMB.lastPositionY;
    }

    // Get stencil index belonging to object where mouse click was read. Flip y coordinate because in GLFW the (0,0) is at top left
    glReadPixels(x, windowHeight - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    return index - 1;
}

void MouseSelection::WindowUpdated(int winW, int winH) {
    windowWidth = winW;
    windowHeight = winH;
}

void MouseSelection::LeftMBClicked(GLFWwindow* win) {

    if (LeftMB.Active)
    {
        LeftMB.Active = false;
    }
    else
    {
        glfwGetCursorPos(win, &LeftMB.lastPositionX, &LeftMB.lastPositionY);
        LeftMB.Active = true;
    }
}

void MouseSelection::RightMBClicked(GLFWwindow* win) {

    if (RightMB.Active)
    {
        RightMB.Active = false;
    }
    else
    {
        glfwGetCursorPos(win, &RightMB.lastPositionX, &RightMB.lastPositionY);
        RightMB.Active = true;
    }
}

glm::vec3 MouseSelection::MouseDrag(glm::mat4 projMatrix, glm::mat4 vMatrix, int mouseIdent) {

    // We need to "unproject" our mouse position to world space...
    glm::vec4 viewPort = glm::vec4(0, 0, windowWidth, windowHeight);
    glm::vec3 winCoord;
    glm::vec3 objCoord;

    double x;
    double y;
    GLfloat tempDepth;

    // Checks if we want to read pixel for left or right mouse button
    if (mouseIdent == 0)
    {
        x = LeftMB.lastPositionX;
        y = LeftMB.lastPositionY;

        glReadPixels(x, windowHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &tempDepth);

        // Read "depth" of mouse position only if we havent yet
        if (LeftMB.keepDepth == false)
        {
            LeftMB.keepDepth = true;
            LeftMB.depth = tempDepth;
        }
        
        winCoord = glm::vec3(x, windowHeight - y - 1, LeftMB.depth);
    }
    else if (mouseIdent == 1)
    {
        x = RightMB.lastPositionX;
        y = RightMB.lastPositionY;

        glReadPixels(x, windowHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &tempDepth);

        // Read "depth" of mouse position only if we havent yet
        if (RightMB.keepDepth == false)
        {
            RightMB.keepDepth = true;
            RightMB.depth = tempDepth;
        }

        winCoord = glm::vec3(x, windowHeight - y - 1, RightMB.depth);
    }

    // We'll return this so its possible to make translations, etc...
    objCoord = glm::unProject(winCoord, vMatrix, projMatrix, viewPort);

    return objCoord;
}

void MouseSelection::ResetDepth(int mouseIdent) {

    if (mouseIdent == 0)
    {
        LeftMB.keepDepth = false;
        LeftMB.depth = NULL;
    }
    else if (mouseIdent == 1)
    {
        RightMB.keepDepth = false;
        RightMB.depth = NULL;
    }
}

