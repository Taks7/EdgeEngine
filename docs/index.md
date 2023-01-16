# EdgeEngine

![EdgeEngine](https://user-images.githubusercontent.com/72123380/198906428-5ab2db1d-a51b-4143-9c39-c5ab097b3490.png)

Edge Engine its a 3D graphics Engine for Game Development developed with C++ and [OpenGL](https://www.opengl.org/). It has been developed by students of the [Degree in Videogame Design and Development](https://www.citm.upc.edu/ing/estudis/graus-videojocs/) of the [Universitat Politècnica de Catalunya](https://www.upc.edu/ca). You can check out our webpage [here](https://taks7.github.io/EdgeEngine/).

# Authors:
- Alejandro Martin:
  - Github Account: [Alejandromo125](https://github.com/Alejandromo125)
- Albert Ramisa:
  - Github Account: [Taks7](https://github.com/taks7)

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
![](ParticleVideoExample.gif)

In this gif we can see all the functionalities of our particles, this allowing the users to create customizable particles for different projects.

### Firework example:
![](FireworkExample.gif)

In this gif we see the firework working, we can create multiple fireworks, for it to be perfect we would also delete the gameObject holding the particle after explosion.

## LICENSE
### MIT LICENSE
![Captura](https://user-images.githubusercontent.com/72123380/198901514-4c3111b2-d181-493a-9e2d-350ec0a65e48.PNG)

Copyright (c) 2022 Albert Ramisa & Alejandro Martin Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

