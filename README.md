## **Shader Project**

The goal of this shader project was to implement a program capable of showcasing stylised materials and lighting models, similarly to a game engine. 
IMAGE


Developed under two weeks, from January 1st to January 15th 2023, this project constituted the final evaluation component of the 2023 edition of the Computer Graphics in Games course, talking place in Instituto Superior Técnico, University of Lisbon.

The following document indicates how to execute and control the program, also describing in further detail its development: from the initial references to the encountered technical challenges. 

## **Build and Execution**

To install this project, follow these steps:

1. Clone the repository: **`git clone https://github.com/zorrocrisis/ShaderProject`**
2. Open the ShaderProject.snl solution using Visual Studio
4. Build the project
5. Start the project (executable should be in Build > Release/Debug)

## **Controls/Features**

+ For example, the HUD could indicate a specific key that, when pressed, would vary the parameters of the cel shading in real-time (which in turn could also be indicated in the transparent overlays). The user could then rotate the objects, pull them far apart or closer together, while varying these parameters, in order to reach a final scene they can be pleased with (almost like a demonstration of a shader program to a potential employer/client). Lastly, once the final scene is achieved, one could save a snapshot (which could include all the registered parameters in the HUD) for future reference and comparison.

## **Materials and Objects**

Two different materials were planned to be developed throughout the course of this project: cork and metal.

Firstly, what better way to represent my home country of Portugal than to develop such a widely known and proudly portuguese material like cork? The goal was to make a stylised/semi-realistic solid material with procedural noise to convey the natural look of cork, which happens to be very uneven and rough. Regarding its application, the cork material was used to make a cork stopper. The following images demonstrate the overall desired look of the material’s surface and its usage in a 3D model of a cork stopper.

![Cork](https://github.com/zorrocrisis/ShaderProject/assets/118909502/19f8e0f5-3854-41f1-b2b8-11f824344bdb)
![cork_stopper_best(1)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/c27611e2-5654-4985-8813-ebf3dfaa3753)

![cork_stopper_model(3)](https://github.com/zorrocrisis/ShaderProject/assets/118909502/ecd45a71-8e0c-48f2-92ac-56c460a71123)

*Image Set 1 – From left to right: [real cork material](https://en.wikipedia.org/wiki/Cork_(material)#/media/File:Cork.jpg), real cork stopper, [3D model with procedural noise material](https://sketchfab.com/3d-models/champagne-cork-ab6482bccb084488b8fdaae82d67ce88)*

Since the project already had a cork stopper, the next logical step was to develop a corkscrew! This corkscrew was planned to be mostly made from metal - more specifically, from stainless steel, which is the preferred material for household kitchen appliances, having a characteristic smoothness and shininess which was attempted to be reproduced (in a stylised version). The following images represent visual references for the material and object desired. The image on the far right is in fact a 3D model of a corkscrew, similar to what is proposed to be developed.



## **Technical Challenges**

The choice of cork and metal, two contrasting materials, gave way to multiple technical challenges which were set as milestones for the project:

A - A stylised cork **material using procedural noise** - As previously mentioned, this challenge had in mind the uneven and natural looking surface of the cork material.

B - Non-photorealistic lighting / shading model with silhouette - The main lighting/shading for this project was based on a stylised, non-photorealistic model, namely, the **Cel shading model**. Visual references for what the final product were taken from the video game industry and other online models:

C - Real-time manipulation of objects with mouse and keyboard

D - Scene graph integration of a Head-Up Display (HUD) with transparent overlays

E - Snapshot saving of the application to a known image file format

These last three challenges allow the scene to be fully explored and interacted with, going hand in hand to showcase the different techniques implemented and the diversity of the program.

## **Development Details**



## **Authors and Acknowledgment**

This shader project was developed by **[Miguel Belbute (zorrocrisis)](https://github.com/zorrocrisis)**.

The initial code was contributed by **[Prof. Carlos Martinho](https://fenix.tecnico.ulisboa.pt/homepage/ist14181)**

## **License**

Shader Project is released under the Attribution 4.0 International License. See the **[LICENSE](https://creativecommons.org/licenses/by/4.0/)** file for details.
