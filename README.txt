This project was implemented by Miguel Belbute for the last assignment of the Computer Graphics for Games course @ Instituto Superior Técnico, University of Lisbon.
==========================================================================================
*** CAMERA CONTROLS ***
P - Switch between projection types - orthographic or perspective.

C - Change active camera.

SCROLL WHEEL - zooms in or out when in projection perspective.

LMB (HOLD) - While holding the LMB (left mouse button), dragging moves the current active camera.

==========================================================================================
*** SELECTION AND REAL TIME MANIPULATION CONTROLS ***
RMB (TOGGLE) - Press the RMB (right mouse button) once over a valid object to select it. After selecting, 
moving the mouse moves the object. You can move the camera freely (with LMB) while RMB is toggled on. 
You can also use the scrollwheel to vary the "depth" of the object's position. Press RMB again to unselect the object.

UP/DOWN arrows - Positive/negative rotation along the y axis of the selected object.

LEFT/RIGHT arrows - Positive/negative rotation along the x axis of the selected object.

./, - Positive/negative rotation along the z axis of the selected object (period/comma).

==========================================================================================
*** MISC CONTROLS ***
M - change cork texture (when cork stopper is selected).

I - randomize light position.

A - change ambient factor.

L - Lock mouse cursor inside window (like an FPS).

U - Unlock mouse cursor.

F - Take screenshot that will be saved as a .png format in the ../assets/screenshots folder.

Esc - Close the window and shut down the application.

==========================================================================================
* REQUIREMENTS/DEPENDENCIES *
assimp version 5.2.5
glew v7.0
glfw v3.3
glm v0.9.9
stb_image v2.27 (header-file library)
stb_image_write v1.16 (header-file library)
freetype v2.12.1

==========================================================================================
* ASSETS * 
arial.ttf (font file downloaded separately. Read by freetype)
corkscrew_smooth.obj
corkstopper_smooth.obj

