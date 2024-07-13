## **Shader Project**

The goal of this shader project was to implement a program capable of showcasing stylised materials and lighting models, similarly to a game engine. 


IMAGE/GIF OF PROGRAM


Developed under two weeks, from January 1st to January 15th 2023, this project constituted the final evaluation component of the 2023 edition of the Computer Graphics in Games course, talking place in Instituto Superior Técnico, University of Lisbon.

The following document indicates how to execute and control the program, also describing in further detail its development: from the initial references to the encountered technical challenges. 

## **Build and Execution**

To install this project, follow these steps:

1. Clone the repository: **`git clone https://github.com/zorrocrisis/ShaderProject`**
2. Open the ShaderProject.snl solution using Visual Studio
4. Build the project
5. Start the project (executable should be in Build > Release/Debug)

## **Controls**

Camera:
P - Switch between projection types - orthographic or perspective.
C - Change active camera.
SCROLL WHEEL - zooms in or out when in projection perspective.
LMB (HOLD) - While holding the LMB (left mouse button), dragging the mouse moves the current active camera.

Selection and Real Time Manipulation:
RMB (TOGGLE) - Press the RMB (right mouse button) once over a valid object to select it. After selecting, moving the mouse moves the object. You can move the camera freely (with LMB) while RMB is toggled on. You can also use the scrollwheel to vary the "depth" of the object's position. Press RMB again to unselect the object.
UP/DOWN arrows - Positive/negative rotation along the y axis of the selected object.
LEFT/RIGHT arrows - Positive/negative rotation along the x axis of the selected object.
./, - Positive/negative rotation along the z axis of the selected object (period/comma).

MISC:
M - change cork texture (when cork stopper is selected).
I - randomize light position.
A - change ambient factor.
L - Lock mouse cursor inside window (like an FPS).
U - Unlock mouse cursor.
F - Take screenshot that will be saved as a .png format in the ../assets/screenshots folder.
Esc - Close the window and shut down the application.


## **Materials and Objects**

Two different materials were planned to be developed throughout the course of this project: cork and metal.

Firstly, what better way to represent my home country of Portugal than to develop such a widely known and proudly portuguese material like cork? The goal was to make a stylised/semi-realistic solid material with procedural noise to convey the natural look of cork, which happens to be very uneven and rough. Regarding its application, the cork material was used to make a cork stopper. The following images demonstrate the overall desired look of the material’s surface and its usage in a 3D model of a cork stopper.

![Cork](https://github.com/zorrocrisis/ShaderProject/assets/118909502/19f8e0f5-3854-41f1-b2b8-11f824344bdb)
![cork_stopper_best(4)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/be64cb5d-e031-46f4-8096-fbd5a6a16dbe)
![cork_stopper_model(3)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/ecd45a71-8e0c-48f2-92ac-56c460a71123)

*Image Set 1 – From left to right: [real cork material](https://en.wikipedia.org/wiki/Cork_(material)#/media/File:Cork.jpg), [real cork stopper](https://www.corklink.com/index.php/agglomeratdor-natural-corks/), [3D model with procedural noise material](https://sketchfab.com/3d-models/champagne-cork-ab6482bccb084488b8fdaae82d67ce88)*

Since the project already had a cork stopper, the next logical step was to develop a corkscrew! This corkscrew was planned to be mostly made from metal - more specifically, from stainless steel, which is the preferred material for household kitchen appliances, having a characteristic smoothness and shininess which was attempted to be reproduced (in a stylised version). The following images represent visual references for the material and object desired. The image on the far right is in fact a 3D model of a corkscrew, similar to what is proposed to be developed.

![stainless_steel4](https://github.com/zorrocrisis/ShaderProject/assets/118909502/b9675617-58a0-4e39-bacd-a5e726a33b04)
![corkscrew(4)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/ff76d9f9-15e6-4636-8d92-0a793817b589)
![corkscrew_model(8)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/7dd16683-2e3b-4892-8252-181d2fbb2a26)

*Images Set 2 – From left to right: [real stainless steel](https://eagletube.com/about-us/news/stainless-steel-characteristics/), [real metallic corkscrew](https://www.theodorebruceauctions.com.au/auction-lot/an-antique-nickel-plate-english-roundlet-travelli_48F4A2EBE9), [3D model with metallic material](https://www.youtube.com/watch?v=NIn421hiFrc)*

## **Technical Challenges**

The choice of cork and metal, two contrasting materials, gave way to multiple technical challenges which were set as milestones for the project:

A - A stylised cork **material using procedural noise** - As previously mentioned, this challenge had in mind the uneven and natural looking surface of the cork material.

B - Non-photorealistic lighting / shading model with silhouette - The main lighting/shading for this project was based on a stylised, non-photorealistic model, namely, the **cel shading model**. Visual references for the final product were taken from the video game industry and other online models:

![wau(4)](https://github.com/user-attachments/assets/c813e2f6-5d4b-4a19-a548-a1131ac03c15)
![cel_unreal(1)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/50eef4fe-cf8a-4ff5-a3d8-7bdab07661ae)
![Using_Toon_052](https://github.com/zorrocrisis/ShaderProject/assets/118909502/13de1655-ef18-4278-aa1a-7e45e005503a)

*Images Set 3 – From left to right: [cork stopper with cel shading used in a video game (The Wolf Among Us)](https://www.youtube.com/watch?v=vxhRINas5S0), two applications of cel shading in metallic materials that convey the sought-after shininess (sources [here](https://www.unrealengine.com/marketplace/en-US/product/stylized-rendering-system-for-mobile-vr-cel-shader) and [here](https://www.reallusion.com/iclone/help/iclone5/pro/10_scene/atmosphere/using_toon_shader.htm))*

C - Real-time manipulation of objects with mouse and keyboard

D - Scene graph integration of a Head-Up Display (HUD) with transparent overlays

E - Snapshot saving of the application to a known image file format

These last three challenges allow the scene to be fully explored and interacted with, going hand in hand to showcase the different techniques implemented and the diversity of the program.

## **Development Details**

First Week (02/01/2023 – 07/01/2023):
- The models for the two objects were created in Blender and were successfully implemented in the scene, inspite of some initial issues regarding face culling;
- The cel shading model was developed according to schedule, although some techniques (like the ones used to draw the silhouettes) were modified and perfected along the second week - objective B;
- Unfortunately, even with multiple attempts, the creation of the cork material was not finished until the end of the first week, being this the only objective not performed according to schedule;
- Before the weekend, an attempt was made to develop a way of taking screenshots of the application. The attempt was successful, but it could only export .tga format images, which is suboptimal;
- Received feedback during practical classes (03/01/2022 and 06/01/2022);

Second Week (08/01/2023 – 12/01/2023):
- A mechanism for the real-time manipulation of the objects through mouse and keyboard inputs was implemented on the weekend and perfected during the first days of the week. Although the approach utili~sed is not optimal, it does present a suitable solution – objective C;
- Another approach was used to try to fulfil the technical challenge of generating a stylised texture using procedural noise. This approach proved to be successful, despite the objective being due the week prior - objective A;
- One other method for saving a screenshot of the application was investigated and developed. The application can now save screenshots to a much more popular and easily accessable image forma (.png) – objective E;
- Lastly, a simple HUD was created to help visualise and control what was happening in the scene. This HUD included camera information, selection information, real-time manipulation controls and the light position.
Sadly, the lack of time did not allow further developments related to visualising shading parameters, for example - objective D;
- Received feedback during practical classes (10/01/2022 and 13/01/2022);
  
Delivery Day (13/01/2023):
- Last minute feedback during the practical class;
- Fine tuned parameters and code structure;
- Performed finishing touches, solved bugs.

## **Program Dependencies and Assets**
- assimp version 5.2.5
- glew v7.0
- glfw v3.3
- glm v0.9.9
- stb_image v2.27 (header-file library)
- stb_image_write v1.16 (header-file library)
- freetype v2.12.1

- arial.ttf (font file downloaded separately. Read by freetype)
- corkscrew_smooth.obj
- corkstopper_smooth.obj

## **Authors and Acknowledgment**

This shader project was developed by **[Miguel Belbute (zorrocrisis)](https://github.com/zorrocrisis)**.

The initial code was contributed by **[Prof. Carlos Martinho](https://fenix.tecnico.ulisboa.pt/homepage/ist14181)**

## **License**

Shader Project is released under the Attribution 4.0 International License. See the **[LICENSE](https://creativecommons.org/licenses/by/4.0/)** file for details.
