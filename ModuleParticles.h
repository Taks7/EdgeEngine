#ifndef __PARTICLE_MODULE_H__
#define __PARTICLE_MODULE_H__

#include "Application.h"
#include "ModuleGameObject.h"
#include "EmitterInstance.h"
#include "Particles.h"
#include "ModuleComponentParticles.h"
#include <string>
#include "Globals.h"
#include "pcg-c-basic-0.9/pcg_basic.h"


struct ReOrderParticles
{
	inline bool operator() (const Particles& particle1, const Particles& particle2)
	{
		return (particle1.distanceToCamera > particle2.distanceToCamera);
	}
};

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

	void Init(EmitterInstance* emitterInstance);
	virtual void Spawn(EmitterInstance* emitterInstance); //It creates a new particle or re-spawns an existing one through object polling
	virtual void Update(EmitterInstance* emitterInstance);
	virtual void Reset();
	virtual void CleanUp();
	void DrawParticles();
	void DeActivateParticles();
	void Save(); //TODO: Save()
	void Load(); //TODO: Load()

	float CalculateParticleDistanceToCamera(Particles* p);
	void UpdateParticleReference(EmitterInstance* emitterInstance);
protected:
	void SortParticles(std::vector<Particles>& particles);
	void CreateParticle(EmitterInstance* emitterInstance); //Function to create new particles
	float3 SetRandomDirection();
	unsigned int GetFirstUnusedParticle();
public:
	Particles* particleReference;
	std::string name;
protected:
	std::vector<Particles> particles_vector;
	uint existing_particles;
	uint activeParticles;
	uint lastUsedParticle;
};

// ---------- CUSTOM PARTICLE ----------

class CustomParticle : public ModuleParticles
{
public:
	CustomParticle(ModuleGameObject* owner);
	~CustomParticle();

	void CleanUp() override;

	Type GetType() { return Type::Custom; };
};

// ---------- SMOKE PARTICLE EMITTER ----------

class Smoke : public ModuleParticles
{
public:
	Smoke(ModuleGameObject* owner);
	~Smoke();

	void CleanUp() override;

	Type GetType() { return Type::Smoke; };
};

// ---------- FIREWORK ----------

class Firework : public ModuleParticles
{
public:
	Firework(ModuleGameObject* owner);
	~Firework();

	void Update(EmitterInstance* emitterInstance) override;
	void Spawn(EmitterInstance* emitterInstance) override;

	void CleanUp() override;

	Color GetRandomColor(range<Color> r);

	Type GetType() { return Type::Firework; };
public:
	ModuleGameObject* fireworkOwner;
private:
	float lifeTime;
	float currentTime;
	range <Color> rangeColor;
};

#endif // !__PARTICLE_MODULE_H__
