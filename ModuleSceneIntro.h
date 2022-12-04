#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeo/src/Geometry/LineSegment.h";
#include <vector>
#include <map>
#include "ModuleGameObject.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"

class Primitive;
class  PhysBody3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Update(float dt) override;
	bool PostUpdate();
	bool CleanUp();

	//TODO 9: Create an "OnCollision" method specific for this module
	ModuleGameObject* CreateEmptyGameObject(const char* name = nullptr, ModuleGameObject* parent = nullptr);
	//RAYCASTING GAMEOBJECTS
	void getRaycastHits(const LineSegment& ray, std::map<float, ModuleGameObject*>& hits);
	bool RaycastSelection(const LineSegment& ray);

	//bool SetSelectedGameObject(ModuleGameObject* game_object);


	float2 getWorldMosuePosition();

	void CreateSceneCamera();

	void SelectItem(ModuleGameObject* game_object);

public:

	std::vector<ModuleGameObject*> game_objects;
	ModuleGameObject* rootObject;
private:
	std::vector<Primitive*> primitives;
};
