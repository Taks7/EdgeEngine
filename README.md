# EdgeEngine

![EdgeEngine](https://user-images.githubusercontent.com/72123380/198906428-5ab2db1d-a51b-4143-9c39-c5ab097b3490.png)

Edge Engine its a 3D graphics Engine for Game Development developed with C++ and [OpenGL](https://www.opengl.org/). It has been developed by students of the [Degree in Videogame Design and Development](https://www.citm.upc.edu/ing/estudis/graus-videojocs/) of the [Universitat Politècnica de Catalunya](https://www.upc.edu/ca). You can check out our webpage [here](https://taks7.github.io/EdgeEngine/).

## Libraries & Tools

- IDE: Microsoft Visual Studio 2022

- Standard Template Library
- SDL 2.0.4
- OpenGL 2.0
- Glew 2.2.0
- ImGui 1.89
- PhysFS 2.0
- MathGeoLib 1.5
- Parson 1.2.1
- Developer's Image Library 1.7.8
- Open Asset Import Library 3.1.1

## Installation

It's easy to instalate the Edge Engine in your computer. Just go in the releases tab and open the assets section, now you can click on the first file and the download will begin. Now you have to compile the executable that you will find in the folder you have just downloaded, and the engine is ready to use :smiley:.

## Engine Features

### UI
![Captura](https://user-images.githubusercontent.com/72123380/198897943-5197fc2b-91a8-4721-a32c-ff061b5348c8.PNG)

- **Help**: In that section we can find the information about the license, developers of the engine and the tools and libraries used. We can also find options to download the latest version or go to the wiki page of the EdgeEngine and finally, there is a button specifically to report a bug.

- **Configuration**: Here you can Save and Load an existing configuration of your Engine Window and Render Options preferences. 

- **Window**: In the window section we can find multiple window configuration options. Here you can toggle on/off fullscreen, resizable, full desktops as well as the vsync or adjust the widht, height and brightness of your screen. It is here that you can also select the ImGui prefrences in the bottom section of the window menu.

- **Hardware**: It displays the information regarding your frame rate(FPS) and your computer hardware.

- **Render Options**: Multiple render options that can be toggled on/off:

  - WireFrame Mode: Transforms a mesh into a wireframe by iterating over its faces, collecting all edges and turning those edges into four-sided polygons.
  
  - GL_Depth_test: Tests the depth value of a fragment against the content of the depth buffer.
  
  - GL_Cull_face: Determines whether a polygon of a graphical object is visible.
  
  - GL_Lightning: Use the current lighting parameters to compute the vertex color.
  
  - GL_Color_material: Specifies which material parameters track the current color.
  
  - GL_Front: Culls only the front faces.
  
  - GL_AmbientOclussion: Calculates how much the pixel is exposed to the ambient light source.
  
- **Game Objects**: Here you can add the game objects to the Engine.

### Drag&Drop

Models and textures can be dropped directly into the engine. That's made by creating a new game object per mesh in the model and the texture will also be applied into that model. Currently EdgeEngine supports .fbx for the objects and dds/.png for the textures.

![DragAndDrop_AdobeExpress (1)](https://user-images.githubusercontent.com/72123380/199331310-b84dbdb1-8b6b-45a9-8159-8478ce12bc6b.gif)

```Caution: You can only drop .fbx and .png files from the Assets folder that is inside the current project.```

### Modify a GameObject

You can completely transform the selected Game Object, being able to rotate, change position and scale it in the three different axis. You can also select and drop an imported mesh, and the same applies to textures that will be automatically applied to the GameObject you want. Finally there is a Camera Component in the scene that will have different settings that can be modified such as the FOV, the Culling or the View distance and it will also feature the above mentioned transform mechanics.


### GameObjects picking & AABB

Game Objects can be picked from the world using the mouse of your computer and the selected mesh will have a bounding volume (AABB) surrounding it and can be discarded with Frustum Culling.

![a2](https://user-images.githubusercontent.com/72123380/206753451-6707725e-0297-47fb-9f25-0701ba0542fa.PNG)

### Save Models, Meshes, Textures and the Scene

You can save your models, meshes and textures using the panel called Inspector that you will find on the left of your screen. The Scene can also be saved to a custom file.

### Play, Pause & Stop

The simulation can be paused and it can also be stoped, where you will recover the original state of all the scene and using the Play you can unpause the simulation again. You will find this functionality on the top of the Screen, with the top navigation bar.

### Two Windows (Editor View & Main Game Camera)

There are two different cameras to be selected, one being the Scene Camera and another being the Game Camera which is a game object with a camera component. They are completely independant and you can adjust the position of both of them to the place you like.

```WIP: We have tested the implementation of showing the two cameras at the same time, but after an extense and difficult progression of that feature we have not achieved it succesfully```

### Resource Management

At the bottom of your screen, next to the console tab there is an Assets window that will display all the user assets. Dropping new assets will modify the window properly and all the resources are only once in memory regardless of how many GameObjects use it.

![a1](https://user-images.githubusercontent.com/72123380/206752955-073635c1-71bb-4936-88b8-252d77bad2cb.PNG)

# Particle system:

For our final part of the project we have created a particle system. With this final module you can create, and modify already existing particles in our engine, and it will be used to complete the engine we will be using for our next big subject: PROJECT 3

## How was it done?

For our particles to work we needed the following things:

*   Base particle class
*   Billboards
*   Emmitter class
*   It's own module

### Base Class
Here we have an example of our base class with all the things needed related to the particles themselves.

```c++
class Particles {
public:
	Particles();
	Particles(Particles* particleReference);
	~Particles();
public:
	float3 position;
	float lifetime;
	float speed;
	float size;
	float dirVariation;
	Color color;
	float3 direction;
	bool active;
	float distanceToCamera;

	ModuleComponentBillBoard* billboard; 
};
```
### Billboard
  The billboard itself what does is allow us to create a base for our particles to be drawn and align them to the camera.
  We have three type of alignments, Screen Alignment, World Alignment and Axis alignment.
  Each gameObject containing particles also has this component as to have it as a base for particles and not having to create multiple gameObjects for the same particle (more or less like instancing)

### Emmitter class

The emmitter class is the one responsible of producing and destroying all the particles needed, being the most important class for the functionality of all our particles. Each particle will be updated thanks to this class

### Module Particles

Here it's where all active particles are handled, more or less like an entities manager but this time only focused on all the particles that we have in game.

## Base particles:

We have created three base particles, one customizable, a smoke particle and a firework. For each particle you can change it's values through the inspector. Some changes available being, changing speeds,directions, colors and even more.
Each particle works like it's own class another time similar to how entities work, each type has it's own variations and can be coded to generate even more like in the firework example.

### Smoke example:

We can modify multiple settings of the particles as the lifetime, the amount of them or its size to say a few. Another functionality added is that the particles created can have the color changed in the inspector tab.

### Firework example:

The fireworks that are spammed with the key 1 are created with enough randomization that the explosion dont always feel the same one. For it to be perfect we would also delete the gameObject holding the particle after explosion.

### Authoring Scenes in the Editor

Functionality of our engine in general,things such as loading meshes and textures, changing parts of the gameObject through the inspector, the hierarchy system and also the capability of swapping textures of gameObjects with just the user interface.

## Engine Controls

- **W**: Move Forward

- **S**: Move Backwards

- **A**: Move Left

- **D**: Move Right

- **R**: Move Upward

- **C**: Move Downward

- **F**: Focuses the Camera Around the Geometry

- **ALTL + Mouse Left Click**: Orbit around the Object

- **Mouse Wheel Forward**: Camera Zoom Forward

- **Mouse Wheel Backward**: Camera Zoom Backwards

- **1**: Create a firework


## Developers

- Alejandro Martin:
  - Github Account: [Alejandromo125](https://github.com/Alejandromo125)

- Albert Ramisa:
  - Github Account: [Taks7](https://github.com/taks7)
  
## LICENSE
### MIT LICENSE
![Captura](https://user-images.githubusercontent.com/72123380/198901514-4c3111b2-d181-493a-9e2d-350ec0a65e48.PNG)

Copyright (c) 2022 Albert Ramisa & Alejandro Martin Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
