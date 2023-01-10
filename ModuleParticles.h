#ifndef __PARTICLE_MODULE_H__
#define __PARTICLE_MODULE_H__
#include "ModuleGameObject.h"
#include "Particles.h"
#include <string>
#include "Globals.h"

class EmitterInstance;

class ModuleParticles {
public:
	enum Type {
		None,
		Custom,
		Smoke,
		Firework
	};

	ModuleParticles();

};

#endif // !__PARTICLE_MODULE_H__
