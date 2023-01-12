#ifndef __ModuleComponentParticles_H__
#define __ModuleComponentParticles_H__

#include "ModuleComponent.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentMesh.h"
#include <vector>
#include <map>
#include "EmitterInstance.h"

#include "Resource.h"
#include "Globals.h"

class ModuleGameObject;

class ModuleComponentParticles : public ModuleComponents {
public:
	ModuleComponentParticles(ModuleGameObject* owner);
	~ModuleComponentParticles();

	bool Init();
	bool Update() override;
	void Reset();

	void Save();
	void Load();

	float GetRandomFloat(range<float> number);
	uint GetRandomUint(range<uint> number);
	
	void AddMaterial(std::map<uint, Resource* > resources);
	void UpdateParticleGUI(Particles* newParticleReference);
public:
	std::vector<EmitterInstance> emitters;

public:
	Particles* particleReference;
	int maxParticles;

	ModuleComponentMaterial* particle_material;

	VertexData* res_mesh;
};
#endif // !__ComponentMesh_H__