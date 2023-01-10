#ifndef __PARTICLES_EMITTER_H__
#define __PARTICLES_EMITTER_H__

#include <vector>
#include "ModuleFBXLoader.h"
#include "ModuleParticles.h"

class ParticleEmitter {
public:
	ParticleEmitter();

	void Save();
	void Load();
public:
	Texture* particleTexture;
	std::vector<ModuleParticles*> modules;
};

#endif // !__PARTICLE_EMITTER_H__