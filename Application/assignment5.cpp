////////////////////////////////////////////////////////////////////////////////
//
//  CGJ - Assignment 5
//
// (c) 2022 by Miguel Belbute - 96453
// (c) 2022 Base Structure by Carlos Martinho
//
// Challenge: Through the implemented real time manipulation mechanisms,
// flip the cork, place it aligned with the corkscrew and rotate the latter.
// In other words, pretend you are opening a wine bottle!
// 
////////////////////////////////////////////////////////////////////////////////

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Assimp
#include <assimp/Importer.hpp>

// STB
#include "stb_image_write.h"

#include "mgl.hpp"
#include "mglTexture.hpp"
#include "mglSampler.hpp"
#include "node.h"
#include "MouseSelection.h"

#include <array>
#include "HUD.h"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void windowCloseCallback(GLFWwindow* window)override;
    void keyCallback(GLFWwindow* win, int key, int scancode, int action,
        int mods) override;
    void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;
    void mouseButtonCallback(GLFWwindow* win, int button, int action,
        int mods) override;
    void scrollCallback(GLFWwindow* win, double xoffset, double yoffset) override;

private:
    // Camera variables
    const GLuint UBO_BP1 = 0;
    mgl::Camera* Camera1 = nullptr;
    mgl::Camera* Camera2 = nullptr;
    mgl::Camera* ActiveCamera = nullptr; 

    // Shaders
    mgl::ShaderProgram* Shader = nullptr;
    mgl::ShaderProgram* TextShader = nullptr;
    mgl::ShaderProgram* OverlayShader = nullptr;

    // Models and meshes
    GLint ModelMatrixIdCorkStopper;
    GLint ModelMatrixIdCorkscrew;
    mgl::Mesh* MeshCorkStopper = nullptr;
    mgl::Mesh* MeshCorkscrew = nullptr;

    // Texture and sampler
    GLuint NoiseTexID;
    mgl::NearestSampler* NearSampler = nullptr;
    mgl::Texture3D* NoiseTexture = nullptr;
    mgl::TextureInfo* TexInfo = nullptr;

    // HUD
    HUD hud;

    // Lever for fragment manipulation
    GLfloat Levers[2] = { 0, 0.45f };

    // Auxiliary variables
    bool camera1BeingUsed = true; // Camera switching
    int scrollLimit = 0; //Scroll detection
    double deltaTime = 0.0; // Camera speed
    double lastFrame = 0.0; // Camera speed
    int windowWidth = 800; // Window size
    int windowHeight = 600; // Window size
    glm::vec3 lightPosition; // Light position

    // Most mouse interactions are handled by this class ( Proud of this :) )
    MouseSelection MouseSelector = { windowWidth, windowHeight };
    GLint ModelIndexSelected = mgl::BACKGROUND; 

    void createMeshes();
    void createTexture();
    void createShaders();
    void createCameras();
    void drawScene();
    void hudDraw();

    /////////////////// Nodes
    node *origin;
    void createNodes();
    void nodesDraw();
    node* SelectNode();
    void deleteNodes(node* current);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    
    std::string mesh_file_cork_stopper = "../assets/models/corkstopper_smooth.obj";
    std::string mesh_file_corkscrew = "../assets/models/corkscrew_smooth.obj";

    MeshCorkStopper = new mgl::Mesh();
    MeshCorkStopper->create(mesh_file_cork_stopper);
    MeshCorkStopper->joinIdenticalVertices();

    MeshCorkscrew = new mgl::Mesh();
    MeshCorkscrew->create(mesh_file_corkscrew);
    MeshCorkscrew->joinIdenticalVertices();

}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaders() {

    /////////////////////////////////////////// Overlay Shader

    OverlayShader = new mgl::ShaderProgram();

    OverlayShader->addShader(GL_VERTEX_SHADER, "overlay-vs.glsl");
    OverlayShader->addShader(GL_FRAGMENT_SHADER, "overlay-fs.glsl");

    // Vertex attribute (position and texture)
    OverlayShader->addAttribute(mgl::VERTEX, 0);

    // Uniform matrices for overlay shader
    OverlayShader->addUniform(mgl::ORTHO_PROJECTION); // orthographically project the overlay on the screen
    OverlayShader->addUniform(mgl::COLOR_UNIFORM);
    OverlayShader->create();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight));

    // Setup some default values of the shader
    OverlayShader->bind();
    glUniformMatrix4fv(OverlayShader->Uniforms[mgl::ORTHO_PROJECTION].index, 1, GL_FALSE, glm::value_ptr(projection));
    OverlayShader->unbind();

    /////////////////////////////////////////// Text Shader

    TextShader = new mgl::ShaderProgram();

    // Add vertex and fragment shaders
    TextShader->addShader(GL_VERTEX_SHADER, "text-vs.glsl");
    TextShader->addShader(GL_FRAGMENT_SHADER, "text-fs.glsl");

    // Vertex attribute (position and texture)
    TextShader->addAttribute(mgl::VERTEX, 0);

    // Uniform matrices for text shader
    TextShader->addUniform(mgl::ORTHO_PROJECTION); // orthographically project the text on the screen
    TextShader->addUniform(mgl::TEXT_TEXTURE); // sampler 2D (texture)
    TextShader->addUniform(mgl::COLOR_UNIFORM);
    TextShader->create();

    // Setup some default values of the shader
    TextShader->bind();
    glUniformMatrix4fv(TextShader->Uniforms[mgl::ORTHO_PROJECTION].index, 1, GL_FALSE, glm::value_ptr(projection));
    TextShader->unbind();

    /////////////////////////////////////////// Cel Shader + Noise

    Shader = new mgl::ShaderProgram();

    // Add vertex and fragment shaders
    Shader->addShader(GL_VERTEX_SHADER, "main-vs.glsl");
    Shader->addShader(GL_FRAGMENT_SHADER, "main-fs.glsl");

    // Position and texture attributes
    Shader->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    Shader->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);

    // Check what type of information the meshes hold (only need to check one)
    if (MeshCorkStopper->hasNormals()) {
        Shader->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    }

    // Uniform matrices for shader
    Shader->addUniform(mgl::NOISE_TEXTURE); // sampler2D
    Shader->addUniform(mgl::TEXTURE_REQUIREMENT); // does this model require a texture?
    Shader->addUniform(mgl::COLOR_UNIFORM); // color
    Shader->addUniform(mgl::LIGHT_POSITION); // light position
    Shader->addUniform(mgl::MODEL_MATRIX); // models
    Shader->addUniform(mgl::HIGHLIGHT_REQUIREMENT); // highlight when a model is selected
    Shader->addUniform(mgl::SILLHOUETTES_OFFSET); // the silhouettes should be slightly bigger than the models
    Shader->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP1); // cameras
    Shader->addUniform(mgl::LEVERS);
    Shader->create();

    ModelMatrixIdCorkStopper = Shader->Uniforms[mgl::MODEL_MATRIX].index; // get corkstopper model id
    ModelMatrixIdCorkscrew = Shader->Uniforms[mgl::MODEL_MATRIX].index; // get corkscrew model id
    NoiseTexID = Shader->Uniforms[mgl::NOISE_TEXTURE].index; // get sampler2D ID

    lightPosition = glm::vec3(0.0f, 0.0f, 5.0f);

    // Setup some default values of the shader
    Shader->bind();
    glUniform3f(Shader->Uniforms[mgl::LIGHT_POSITION].index, lightPosition.x, lightPosition.y, lightPosition.z); // initialize light position for all
    glUniform1fv(Shader->Uniforms[mgl::LEVERS].index, 2, Levers); // initialize levers for fragment manip
    Shader->unbind();
    
}

///////////////////////////////////////////////////////////////////////// TEXTURE

void MyApp::createTexture() {

    // Create noise texture and noise sampler
    NearSampler = new mgl::NearestSampler();
    NoiseTexture = new mgl::Texture3D;

    // Generate texture (in this case, white noise)
    NoiseTexture->createWhiteNoise(256, 256, 256); // width, height and depth

    // Since we only have 1 texture, its GL_TEXTURE0
    TexInfo = new mgl::TextureInfo(GL_TEXTURE0, NoiseTexID, mgl::NOISE_TEXTURE, NoiseTexture, NearSampler);
    MeshCorkStopper->generateTexcoords();

}

///////////////////////////////////////////////////////////////////////// CAMERAS

// Default camera/eye positions
glm::vec3 defaultEyePosition1 = glm::vec3(0.0f, 0.0f, 15.0f);
glm::vec3 defaultEyePositon2 = glm::vec3(0.0f, 15.0f, 0.0f);

// Default center (0.0, 0.0, 0.0) and ups
glm::vec3 defaultCenter = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 defaultUp1 = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 defaultUp2 = glm::vec3(0.0f, 0.0f, 1.0f);

// Eye(0,0,5) Center(0,0,0) Up(0,1,0)
glm::mat4 ViewMatrix1 = glm::lookAt(defaultEyePosition1, defaultCenter, defaultUp1);

// Eye(5,0,0) Center(0,0,0) Up(0,0,1)
glm::mat4 ViewMatrix2 = glm::lookAt(defaultEyePositon2, defaultCenter, defaultUp2);

// Orthographic LeftRight(-10,10) BottomTop(-10,10) NearFar(1,90)
const glm::mat4 ProjectionMatrix1 =
glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 10.0f, 90.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(90)
glm::mat4 ProjectionMatrix2 =
glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 0.5f, 90.0f);

// Default camera speed
float defaultCameraSpeed = 0.05f;

void MyApp::createCameras() {

    // Camera 1 looks directly at wolf configuration plane xy
    Camera1 = new mgl::Camera(UBO_BP1);
    Camera1->setViewMatrix(ViewMatrix1);
    Camera1->setProjectionMatrix(ProjectionMatrix1);
    Camera1->storeEyePosition(defaultEyePosition1);

    // Camera 2 looks directly at box configuration plane xz
    Camera2 = new mgl::Camera(UBO_BP1);
    Camera2->setViewMatrix(ViewMatrix2);
    Camera2->setProjectionMatrix(ProjectionMatrix2);
    Camera2->storeEyePosition(defaultEyePositon2);

    // Setup active camera (default to camera 2)
    ActiveCamera = new mgl::Camera(UBO_BP1);
    ActiveCamera->setViewMatrix(Camera2->getViewMatrix());
    ActiveCamera->setProjectionMatrix(Camera2->getProjectionMatrix());
    ActiveCamera->storeEyePosition(Camera2->getEyePosition());

    camera1BeingUsed = false;
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {

    // Drawing of the models
    nodesDraw();

    // Drawing of the HUD
    hudDraw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {

    // This prevents a bug where moving selected objects would disappear if window wasn't updated...
    MouseSelector.WindowUpdated(windowWidth, windowHeight);

    // Allows creating random light positions 
    srand(time(0));

    // Important stuff
    createMeshes();
    createShaders();
    createTexture();
    createCameras();
    createNodes();

    // Hud setup
    hud.init(TextShader, OverlayShader, mgl::COLOR_UNIFORM, mgl::ORTHO_PROJECTION);
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) { // Window resize

    // Check whether width or height is bigger so we can adjust viewport accordingly
    if (winx > winy)
    {
        glViewport(0, -(winx - winy) / 2.0, winx, winx);
    }
    else
    {
        glViewport(-(winy - winx) / 2.0, 0, winy, winy);
    }

    // Store information about window size
    windowWidth = winx;
    windowHeight = winy;

    MouseSelector.WindowUpdated(windowWidth, windowHeight);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action,
    int mods) {

    /* KEY CONTROLS
       P - switch between projection types - orthographic or perspective
       C - change active camera
       L - lock mouse cursor inside window
       U - unlock mouse cursor
       UP/DOWN arrows - positive/negative rotation along the y axis of the selected object
       LEFT/RIGHT arrows - positive/negative rotation along the x axis of the selected object
       ./, - positive/negative rotation along the z axis of the selected object
       M - change cork texture (when corkstopper is selected)
       I - randomize light position
       A - change ambient light
       F - take screenshot (.png format)
       Esc - close the window
    */

    //// CAMERAS
    if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) //Switch between projection matrices
    {
        // Check what projection matrix were using and switch to the opposite one
        if (glm::all(glm::equal(ActiveCamera->getProjectionMatrix(), ProjectionMatrix1, 0.0001f)))
        {
            ActiveCamera->setProjectionMatrix(ProjectionMatrix2);
            std::cout << "\nPerspective projection is now active...\n";
        }
        else if (glm::all(glm::equal(ActiveCamera->getProjectionMatrix(), ProjectionMatrix2, 0.0001f)))
        {
            ActiveCamera->setProjectionMatrix(ProjectionMatrix1);
            std::cout << "\nOrthographic projection is now active...\n";
        }
    }
    else if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS) //Switch between cameras
    {

        // Check what camera is being used and switch to the opposite one
        if (camera1BeingUsed)
        {
            // Store camera 1 information
            Camera1->setViewMatrix(ActiveCamera->getViewMatrix());
            Camera1->setProjectionMatrix(ActiveCamera->getProjectionMatrix());
            Camera1->storeEyePosition(ActiveCamera->getEyePosition());

            // Change shader's camera to camera 2
            Shader->bind();
            glBindBuffer(GL_UNIFORM_BUFFER, UBO_BP1);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                glm::value_ptr(Camera2->getViewMatrix()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            Shader->unbind();

            Shader->bind();
            glBindBuffer(GL_UNIFORM_BUFFER, UBO_BP1);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                glm::value_ptr(Camera2->getProjectionMatrix()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            Shader->unbind();

            // Update active camera with information from camera 2
            ActiveCamera->setViewMatrix(Camera2->getViewMatrix());
            ActiveCamera->setProjectionMatrix(Camera2->getProjectionMatrix());
            ActiveCamera->storeEyePosition(Camera2->getEyePosition());

            camera1BeingUsed = false;
            std::cout << "Camera 2 is now active..." << std::endl;
        }
        else
        {
            // Store camera 2 information
            Camera2->setViewMatrix(ActiveCamera->getViewMatrix());
            Camera2->setProjectionMatrix(ActiveCamera->getProjectionMatrix());
            Camera2->storeEyePosition(ActiveCamera->getEyePosition());

            // Change shader's camera to camera 1
            Shader->bind();
            glBindBuffer(GL_UNIFORM_BUFFER, UBO_BP1);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                glm::value_ptr(Camera1->getViewMatrix()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            Shader->unbind();

            Shader->bind();
            glBindBuffer(GL_UNIFORM_BUFFER, UBO_BP1);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                glm::value_ptr(Camera1->getProjectionMatrix()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            Shader->unbind();

            // Update active camera with information from camera 1
            ActiveCamera->setViewMatrix(Camera1->getViewMatrix());
            ActiveCamera->setProjectionMatrix(Camera1->getProjectionMatrix());
            ActiveCamera->storeEyePosition(Camera1->getEyePosition());

            camera1BeingUsed = true;
            std::cout << "Camera 1 is now active..." << std::endl;
        }
    }

    //// CURSOR
    if (key == GLFW_KEY_L && action == GLFW_PRESS) { // Lock the cursor inside the window (good for fully rotating the pieces)
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::cout << "\nCursor is now locked...\n";
    }
    else if (key == GLFW_KEY_U && action == GLFW_PRESS) { // Unlock the cursor
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout << "\nCursor is now unlocked...\n";
    }

    //// MANIPULATION 
    if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) // Positive rotation along y axis // FIND SOLUTION FOR DIFFERENT ROTATIONS
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

            // Get selected object and rotate it
            SelectNode()->addYRotation(0.05f);
        }
    }
    else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) // Negative rotation along y axis
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

            // Get selected object and rotate it
            SelectNode()->addYRotation(-0.05f);
        }
    }

    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) // Positive rotation along x axis 
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {
            
            SelectNode()->addXRotation(0.05f);
        }
    }
    else if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) // Negative rotation along x axis 
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

            SelectNode()->addXRotation(-0.05f);
        }
    }

    if (glfwGetKey(win, GLFW_KEY_PERIOD) == GLFW_PRESS) // Positive rotation along z axis
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

            SelectNode()->addZRotation(0.05f);
        }
    }
    else if (glfwGetKey(win, GLFW_KEY_COMMA) == GLFW_PRESS) // Negative rotation along z axis
    {
        if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

            SelectNode()->addZRotation(-0.05f);
        }
    }

    if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) { // Change cork texture (different styles)

        if (MouseSelector.RetrieveReference(ModelIndexSelected) != "Cork")
            return;

        Levers[0] = Levers[0] + 1;
        if (Levers[0] == 4) Levers[0] = 0; // loop through different styles

        Shader->bind();
        glUniform1fv(Shader->Uniforms[mgl::LEVERS].index, 2, Levers); // update levers
        Shader->unbind();
    }

    if (glfwGetKey(win, GLFW_KEY_I) == GLFW_PRESS) { // Randomize light position


        lightPosition = glm::vec3(rand()%10, rand() % 10, rand() % 10);

        Shader->bind();
        glUniform3f(Shader->Uniforms[mgl::LIGHT_POSITION].index, lightPosition.x, lightPosition.y, lightPosition.z);
        Shader->unbind();

    }

    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { // Increase ambient factor

        Levers[1] = Levers[1] + 0.05f;

        if (Levers[1] > 1.0f)
            Levers[1] = 0.0f;

        Shader->bind();
        glUniform1fv(Shader->Uniforms[mgl::LEVERS].index, 2, Levers);
        Shader->unbind();

    }

    //// SCREENSHOT
    if (key == GLFW_KEY_F && action == GLFW_PRESS) { // Take and save screenshot (.png format)

        // Older method, deprecated due to unpopular .tga format
        {
       /*
       //Uncompressed true color tga header
       GLubyte TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

       GLubyte infoHeader[6];

       // Create space for the data considering the window dimensions
       unsigned char* data = new unsigned char[4 * windowWidth * windowHeight];

       if (!data)
       {
           std::cout << "Unable to allocate space for screen data" << std::endl;
           return;
       }

       // Read the data we want to "transfer" to the screenshot
       // We use the front buffer for the anti-aliasing
       glReadBuffer(GL_FRONT);
       glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
       glReadBuffer(GL_BACK);

       //Data needs to be in BGR format. Since we read the data with GL_RGBA, we have to swap the B and R
       for (int i = 0; i < (int)windowWidth * windowHeight * 4; i += 4)
       {
           // Repeated XOR operation to swap bytes
           data[i] ^= data[i + 2] ^= data[i] ^= data[i + 2];
       }

       // Open the file ("wb" -> for writing non-text files)
       err = fopen_s(&streamWrite, filename, "wb");

       // Save TGA header
       fwrite(TGAheader, 1, sizeof(TGAheader), streamWrite);

       // Fill out info header
       infoHeader[0] = (windowWidth & 0x00FF);
       infoHeader[1] = (windowWidth & 0xFF00) >> 8;
       infoHeader[2] = (windowHeight & 0x00FF);
       infoHeader[3] = (windowHeight & 0xFF00) >> 8;
       infoHeader[4] = 32;
       infoHeader[5] = 0;

       // Save info header
       fwrite(infoHeader, 1, sizeof(infoHeader), streamWrite);

       // Save the previously obtained data (from the readbuffer)
       fwrite(data, 1, windowWidth * windowHeight * 4, streamWrite);

       fclose(streamWrite);

       // Free up data
       if (data)
           delete[] data;
       data = NULL;

       std::cout << "The screenshot has been successfully saved!" << std::endl;
       return;
       */
        }

        // New method, supported by the stb library
        FILE* streamRead, * streamWrite;
        errno_t err;
        char filename[40];

        // Check and generate possible file name
        for (int i = 0; i < 100; ++i)
        {
            // Generate a filename with index i
            sprintf_s(filename, "../assets/screenshots/screenshot%02d.png", i);

            // Try opening this file with the generated name ("rb" -> for reading/opening non-text files)
            err = fopen_s(&streamRead, filename, "rb");

            // If we cant open the file, it means it hasnt been created yet! Use this file name
            if (err == 2)
                break;

            // Otherwise, the file exists. Try the next generated name...
            fclose(streamRead);

            // ... unless we reached the end of the possible number of files
            if (i == 99)
            {
                std::cout << "No space to save screenshot!" << std::endl;
                return;
            }
        }

        std::cout << "Saving: " << filename << std::endl;

        // Create space for the data considering the window dimensions and number of channels
        std::vector<unsigned char> pixels(windowWidth * windowHeight * 4);

        // Read the pixel data from the frame buffer
        glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        // Flip the image vertically (top left vs bottom left conventions)
        for (int y = 0; y < windowHeight / 2; y++) {
            for (int x = 0; x < windowWidth; x++) {
                
                int top = (y * windowWidth + x) * 4;
                int bottom = ((windowHeight - y - 1) * windowWidth + x) * 4;
                
                for (int i = 0; i < 4; i++) {
                    std::swap(pixels[top + i], pixels[bottom + i]);
                }
            }
        }

        // Save the data read from the buffer into a PNG file (using stb library)
        int strideInBytes = 4 * windowWidth;
        if (!stbi_write_png(filename, windowWidth, windowHeight, 4, pixels.data(), strideInBytes)) {
            
            std::cerr << "Failed to save screenshot to " << filename << std::endl;
        }

        std::cout << "The screenshot has been successfully saved!" << std::endl;

    }

    //// QUIT
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {  // Pressing escape will close the window
        glfwSetWindowShouldClose(win, GLFW_TRUE);
        windowCloseCallback(win);
    }

}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {

    // Allows rotation when left mouse is pressed
    if (MouseSelector.LeftMB.Active)
    {
        // Get angle increments/deltas regarding window size
        double deltaAngleX = (glm::pi<double>() / windowWidth);
        double deltaAngleY = (glm::pi<double>() / windowHeight);

        // Calculate angle of rotation considering previous and present mouse position
        double xAngle = (MouseSelector.LeftMB.lastPositionX - xpos) * deltaAngleX;
        double yAngle = (MouseSelector.LeftMB.lastPositionY - ypos) * deltaAngleY;

        ///////// Rotate the camera using the up vector as an axis - left to right rotation

        // Approach using quaternions (avoiding Gimbal Lock)
        glm::quat quaternionRotationX = glm::angleAxis((float)xAngle, glm::normalize(ActiveCamera->getUp()));
        glm::mat4 rotationMatrixX = glm::toMat4(quaternionRotationX);

        glm::vec4 homogenousEyePosition1(ActiveCamera->getEyePosition(), 1.0f);
        glm::vec3 tempPosition = rotationMatrixX * homogenousEyePosition1;

        ///////// Rotate the camera using the right vector as an axis - top to bottom rotation

        // Approach using quaternions (avoiding Gimbal Lock)
        glm::quat quaternionRotationY = glm::angleAxis((float)yAngle, glm::normalize(ActiveCamera->getRight()));
        glm::mat4 rotationMatrixY = glm::toMat4(quaternionRotationY);

        glm::vec4 homogenousEyePosition2(tempPosition, 1.0f);
        glm::vec3 finalPosition = rotationMatrixY * homogenousEyePosition2;

        ///////// Update the camera view matrix (we keep the default center but cant use the default up)
        glm::mat4 newViewMatrix = glm::lookAt(finalPosition, defaultCenter, ActiveCamera->getUp());
        ActiveCamera->storeEyePosition(finalPosition);
        ActiveCamera->setViewMatrix(newViewMatrix);

        // Update the mouse position for the next rotation
        MouseSelector.LeftMB.lastPositionX = xpos;
        MouseSelector.LeftMB.lastPositionY = ypos;
    }

    // If RMB is selected and it isn't selecting the background...
    if (MouseSelector.RightMB.Active && ModelIndexSelected != mgl::BACKGROUND) {

        // Update mouse position "manually" because we want the cursor to remain active
        MouseSelector.RightMB.lastPositionX = xpos;
        MouseSelector.RightMB.lastPositionY = ypos;

        glm::mat4 view = ActiveCamera->getViewMatrix();
        glm::mat4 proj = ActiveCamera->getProjectionMatrix();

        // Get mouse position in world coordinates
        glm::vec3 where2Go = MouseSelector.MouseDrag(proj, view, mgl::RIGHTMOUSE);

        // Select node associated with ModelIndexSelected and translate it according to mouse movement
        SelectNode()->translateModel(where2Go);
    }
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action,
    int mods) {

    /* MOUSE CONTROLS
       LMB (HOLD) - dragging the mouse moves the camera
       RMB (TOGGLE) - select/unselect object (if valid), dragging the mouse moves the object
    */

    // LEFT mouse is pressed (button code is 0) -> Allows camera movement
    if (button == mgl::LEFTMOUSE)
    {
        // Handles mouse position update and release
        MouseSelector.LeftMBClicked(win);
    }
    // RIGHT mouse is pressed (button code is 1) -> Allows selection of objects
    else if (button == mgl::RIGHTMOUSE && action == mgl::PRESS_DOWN)
    {
        // Handles mouse position update and release
        MouseSelector.RightMBClicked(win);

        if (MouseSelector.RightMB.Active)
        {
            // Well use the stencil buffer to detect what object we selected
            ModelIndexSelected = MouseSelector.ReadPixel(mgl::RIGHTMOUSE);

            if (ModelIndexSelected == mgl::BACKGROUND || ModelIndexSelected == mgl::OVERLAY)
            {
                std::cout << "You can't select the background..." << std::endl;
                MouseSelector.RightMB.Active = false;
            }
            else
            {
                std::cout << MouseSelector.RetrieveReference(ModelIndexSelected) << " model selected!" << std::endl;
                SelectNode()->NodeNeedsHighlight = true; // we need to highlight the model
            }
        }
        else
        {
            if (ModelIndexSelected != mgl::BACKGROUND && ModelIndexSelected != mgl::OVERLAY)
            {
                std::cout << MouseSelector.RetrieveReference(ModelIndexSelected) << " model unselected!" << std::endl;
                MouseSelector.ResetDepth(mgl::RIGHTMOUSE); // reset mouse depth
                SelectNode()->NodeNeedsHighlight = false; // no longer need highlight
                ModelIndexSelected = mgl::BACKGROUND; // reset index to background
            }
                
        }

    }

}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {

    // Make scroll speed independent of machine
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = defaultCameraSpeed * (float)deltaTime;

    // Zoom factors
    float zoomInFactor = 0.9f; //-10%
    float zoomOutFactor = 1.1f; //+10%

    //Get current eye position
    glm::vec3 currentEye = ActiveCamera->getEyePosition();

    // Multiply eye position with factor so we can zoom in and zoom out
    if (yoffset > scrollLimit) //Zoom in
    {
        currentEye *= zoomInFactor;
    }
    else if (yoffset < scrollLimit) //Zoom out
    {
        currentEye *= zoomOutFactor;
    }

    // Udate current eye position and view matrix
    const glm::mat4 newViewMatrix = glm::lookAt(currentEye, defaultCenter, ActiveCamera->getUp());
    ActiveCamera->storeEyePosition(currentEye);
    ActiveCamera->setViewMatrix(newViewMatrix);

}

void MyApp::windowCloseCallback(GLFWwindow* window) {
    deleteNodes(origin);
    delete MeshCorkStopper;
    delete MeshCorkscrew;
    delete Shader;
    delete OverlayShader;
    delete TextShader;
    delete NoiseTexture;
    delete NearSampler;
    delete TexInfo;
    delete Camera1;
    delete Camera2;
    delete ActiveCamera;
}

///////////////////////////////////////////////// NODES

void MyApp::createNodes() {
    
    // Meshes and models IDs
    mgl::Mesh* Meshes[2] = {MeshCorkStopper, MeshCorkscrew};
    GLint IDs[2] = {ModelMatrixIdCorkStopper, ModelMatrixIdCorkscrew};

    // Initial translations (no need messing with scales and rotations)
    glm::vec3 translations[2] = { glm::vec3(7.f, 0.f, 0.f), mgl::NO_TRANSLATION };

    // RGB colours of cork and metal to vec3...
    glm::vec3 color[2] = {glm::vec3(190.0f/255.0f, 142.0f/255.0f, 87.0f/255.0f), glm::vec3(198.0f/255.0f, 198.0/255.0f, 198.0/255.0f)};

    // Does the model need any texture?
    int needsTexture[2] = { 1, 0 };
    
    // Cork Stopper
    origin = new node(Meshes[0], Shader, IDs[0], translations[0], mgl::NO_ROTATION, mgl::NO_SCALE, color[0], needsTexture[0], NoiseTexture, TexInfo);
    node* current = origin;

    // Corkscrew
    current->linkChild(new node(Meshes[1], Shader, IDs[1], translations[1], mgl::NO_ROTATION, mgl::NO_SCALE, color[1], needsTexture[1], NoiseTexture, TexInfo));

    current = current->next_level;

    origin->initializeModels();
}

void MyApp::deleteNodes(node* current) {
    node* next;
    next = current->next_level;
    delete current;
    if (next != NULL)deleteNodes(next);
}

node* MyApp::SelectNode() {

    // Select node corresponding to the selected object
    node* current = origin;

    for (int i = 0; i < ModelIndexSelected; i++)
    {
        current = current->next_level;
    }

    return current;
}

///////////////////////////////////////////////// DRAW

void MyApp::nodesDraw() {
    node* curr;
    curr = origin;

    MouseSelector.ConfigureStencilForSelection();
    
    // Drawing cork model
    MouseSelector.AssignReference(0, "Cork");
    curr->drawCelSelf();

    curr = curr->next_level;

    // Drawing corkscrew model
    MouseSelector.AssignReference(1, "Corkscrew");
    curr->drawCelSelf();
}

///////////////////////////////////////////////// HUD

void MyApp::hudDraw() {

    MouseSelector.AssignReference(-2, "Overlay"); // Assign reference to overlay so we can't select it

    // Print everything we need... (positions are given as percentage)
    // Note: These "magic numbers" result mainly from trial and error
    glm::vec2 selectionTextPos(0.02f, 0.92f);
    glm::vec2 cameraTextPos(0.78f, 0.92f);
    glm::vec2 perspectiveTextPos(0.785f, 0.88f);
    glm::vec2 pressTextPos(0.78f, 0.78f);
    glm::vec2 lightTextPos(0.02f, 0.85f);
    glm::vec2 ambientTextPos(0.02f, 0.80f);
    glm::vec2 diffuseTextPos(0.02f, 0.75f);

    glm::vec2 overlayCamPos(0.765f, 0.85f); // for camera text
    glm::vec2 overlaySelectPos(0.01f, 0.89f); // for selection text
    glm::vec2 overlayButtonsPos(0.765f, 0.55f); // for selection text

    std::string text = "";

    glm::vec3 red(0.8f, 0.0f, 0.0f);
    glm::vec3 green(0.0f, 0.4f, 0.0f);
    glm::vec3 white(0.9f, 0.9f, 0.9f);

    // Note: scales are in function of the windowWidth in order to grow with size

    // Print selected model
    if (ModelIndexSelected != mgl::BACKGROUND && ModelIndexSelected != mgl::OVERLAY)
    {
        text = MouseSelector.RetrieveReference(ModelIndexSelected) + " selected!";
        hud.renderText(text, selectionTextPos, 0.0009 * windowWidth, green);
    }
    else
    {
        text = "No object selected...";
        hud.renderText(text, selectionTextPos, 0.0009 * windowWidth, red);
    }

    // Print camera being used
    if (camera1BeingUsed) text = "Camera 1";
    else text = "Camera 2";
    hud.renderText(text, cameraTextPos, 0.0009 * windowWidth, red);

    // Print perspective being used
    if (glm::all(glm::equal(ActiveCamera->getProjectionMatrix(), ProjectionMatrix1, 0.0001f)))
        text = "Orthographic pers.";
    else if (glm::all(glm::equal(ActiveCamera->getProjectionMatrix(), ProjectionMatrix2, 0.0001f)))
        text = "Projection pers.";
    hud.renderText(text, perspectiveTextPos, 0.0005 * windowWidth, red);

    // Print light position
    std::string x = std::to_string((int)lightPosition.x);
    std::string y = std::to_string((int)lightPosition.y);
    std::string z = std::to_string((int)lightPosition.z);
    text = "Light is at (" + x + "," + y + "," + z + ")";
    hud.renderText(text, lightTextPos, 0.0005 * windowWidth, red);

    // Print ambient factor
    text = "Ambient factor: " + std::to_string((float)Levers[1]);
    hud.renderText(text, ambientTextPos, 0.0004 * windowWidth, red);

    // Print diffuse factor
    text = "Diffuse factor: " + std::to_string(1.0f - (float)Levers[1]);
    hud.renderText(text, diffuseTextPos, 0.0004 * windowWidth, red);

    // Print transparent overlays
    hud.renderOverlay(overlayCamPos, 180/800.0f * windowWidth, 80/800.0f * windowWidth, white);
    hud.renderOverlay(overlaySelectPos, 335 / 800.0f * windowWidth, 55 / 800.0f * windowWidth, white);

    // Print buttons when valid object is selected
    if (ModelIndexSelected != mgl::BACKGROUND && ModelIndexSelected != mgl::OVERLAY)
    {
        text = "Try these...";
        hud.renderText(text, pressTextPos, 0.0007f * windowWidth, green);

        text = "UP/DOWN (rot)";
        hud.renderText(text, glm::vec2(0.785f, 0.73f), 0.0005f * windowWidth, green);

        text = "LEFT/RIGHT (rot)";
        hud.renderText(text, glm::vec2(0.785f, 0.68f), 0.0005f * windowWidth, green);

        text = ". /, (rot)";
        hud.renderText(text, glm::vec2(0.785f, 0.63f), 0.0005f * windowWidth, green);

        if (MouseSelector.RetrieveReference(ModelIndexSelected) == "Cork") // cork has one extra button...
        {
            text = "M (texture)";
            hud.renderText(text, glm::vec2(0.785f, 0.58f), 0.0005f * windowWidth, green);
        }

        hud.renderOverlay(overlayButtonsPos, 180 / 800.0f * windowWidth, 170 / 800.0f * windowWidth, white);
    }
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(800, 600, "Mesh Loader", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////

