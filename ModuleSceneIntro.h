#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

class Primitive;
class  PhysBody3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	//TODO 9: Create an "OnCollision" method specific for this module

private:
	p2DynArray<Primitive*> primitives;
};
