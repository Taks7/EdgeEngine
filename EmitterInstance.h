#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__

#include "Globals.h"
#include <vector>
#include "Particles.h"
#include <map>
#include <algorithm>

class ModuleComponentParticles;
class ParticleEmitter;

class EmitterInstance
{
public:
	EmitterInstance(ParticleEmitter* emitterReference);

	void Init();
	void UpdateModules();
	void Draw();
	void UpdateParticleReference();
	void Reset();
public:
	ParticleEmitter* emitter;
	ModuleComponentParticles* owner;
};
#endif // !__EMITTER_INSTANCE_H__