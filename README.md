## **Shader Project**

This document aims to detail the proposed goals, technical challenges, development plan and execution of the a shader project of the Computer Graphics in Games course, which took place in Instituto Superior Técnico, University of Lisbon.


## **Installation**

To install this project, follow these steps:

1. Clone the repository: **`git clone https://github.com/zorrocrisis/ShaderProject`**
2. Open the ShaderProject.snl solution using Visual Studio
4. Build the project
5. Start the project (executable should be in Build > Release/Debug)

## **Materials and Objects**

Two different materials were planned to be developed throughout the course of this project: cork and metal. The choice of these contrasting materials gives way to multiple technical challenges, which are also pointed out later in this document.

Firstly, what better way to represent Portugal than to develop such a widely known and proudly portuguese material like cork? The goal is to make a stylised/semi-realistic solid material with procedural noise to convey
the natural look of cork, which happens to be very uneven and rough. Regarding its application, the cork material will be used to make a cork stopper (sometimes simply called “cork”, as well). The following images demonstrate the overall desired look of the material’s surface and its usage in a 3D model of a cork stopper.

https://en.wikipedia.org/wiki/File:Cork.jpg
![imagem](https://github.com/zorrocrisis/ShaderProject/assets/118909502/55e3e966-dfe2-4684-88bf-cf8f6934cc67)
![imagem](https://github.com/zorrocrisis/ShaderProject/assets/118909502/c530acfd-91ac-4472-b80c-f06f5cc6ae83)

Image Set 1 – From left to right: real cork, real cork stopper, 3D model with procedural noise material.

Secondly, since we already have a cork stopper, the next logical step is to develop a corkscrew! This corkscrew would be mostly made from metal - more specifically, from stainless steel, which is the preferred material for household kitchen appliances and has a characteristic smoothness and shininess that is aimed to be reproduced (in a stylised version). The following images represent visual references for the material and object desired. The image on the far right is in fact a 3D model of a corkscrew, similar to what is proposed to be developed.

## **Technical Challenges**

Considering the previously indicated information about the objects and materials proposed to be developed, we can list the technical challenges expected to be encountered and a few extra suggested implementations:

A - Stylised solid material using procedural noise - As previously mentioned, this challenge would be specifically for the making of the uneven and natural looking surface of the cork material.

B - Non-photorealistic lighting / shading model with silhouette - The main lighting/shading for this project is based on a stylised, non-photorealistic model, namely, the Cel shading model. Visual references for what is desired can be taken from the video game industry and other online models:

C - Real-time manipulation of objects with mouse and keyboard;

D - Scene graph integration of a Head-Up Display (HUD), e.g. transparent overlays;

E - Saving a snapshot of the application to a known image file format;

+ These last three challenges would allow the scene to be fully explored and interacted with, going hand in hand to showcase the user the different techniques implemented and the diversity of the program. For example, the HUD could indicate a specific key that, when pressed, would vary the parameters of the cel shading in real-time (which in turn could also be indicated in the transparent overlays). The user could then rotate the objects, pull them far apart or closer together, while varying these parameters, in order to reach a final scene they can be pleased with (almost like a demonstration of a shader program to a potential employer/client). Lastly, once the final scene is achieved, one could save a snapshot (which could include all the registered parameters in the HUD) for future reference and comparison.


## **Development Plan**
This project was accomplished in a time frame of two weeks, in the beginning of January 2023.


## **License**

Shader Project is released under the Attribution 4.0 International License. See the **[LICENSE](https://creativecommons.org/licenses/by/4.0/)** file for details.

## **Authors and Acknowledgment**

Project Title was created by **[Miguel Belbute (zorrocrisis)](https://github.com/zorrocrisis)**.

The initial code was contributed by **[Prof. Carlos Martinho](https://fenix.tecnico.ulisboa.pt/homepage/ist14181)**

