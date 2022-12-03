#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Module.h"
#include "ModuleComponent.h"
#include "MathGeo/src/Geometry/AABB.h"
#include "MathGeo/src/Geometry/OBB.h"
#include <vector>
#include <map>
#include <string>

typedef unsigned int uint;

enum class COMPONENT_TYPES;

class ModuleComponents;
class ModuleGameObject
{
public:

	ModuleGameObject(uint id, std::string name, bool isActive, bool isStatic);

	// Destructor
	virtual ~ModuleGameObject();

	bool Update();
	void Render();
	bool CleanUp();
	bool IsActive();
	void SetActive(bool state);
	bool IsStatic();
	void SetStatic(bool state);
	bool IsSelected();

	void SetBB(bool state);
	bool IsBBActive();

	

	bool AddChild(ModuleGameObject* child);
	bool DeleteChild(ModuleGameObject* child);

	std::string GetName();
	std::string GetMeshPath();
	std::string GetTexturePath();
	void SetName(const char* newName);
	ModuleComponents* GetComponent(COMPONENT_TYPES type);
	ModuleComponents* CreateComponent(COMPONENT_TYPES type);
	void UpdateBoundingBoxes();

	AABB GetAABB();

public:

	std::vector<ModuleComponents*>	components;
	std::vector<ModuleGameObject*>	childs;
	
	ModuleGameObject* parent;


	//FOR BOUNDING BOXES will be used later

	OBB	obb;
	AABB aabb;

	float3* obb_vertices;
	float3* aabb_vertices;

	std::string name;
	bool is_active;
	bool is_static;
	bool selectedForInspector = false;
	bool bouindingBoxes = false;
private:
	uint id;
	
};

#endif // __ModuleGameObject_H__