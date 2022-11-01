#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Module.h"
#include "ModuleComponent.h"
#include <vector>
#include <map>
#include <string>

typedef unsigned int uint;
class ModuleComponents;
enum class COMPONENT_TYPES;

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

	void SelectItem();

	bool AddChild(ModuleGameObject* child);
	bool DeleteChild(ModuleGameObject* child);

	std::string GetName();
	std::string GetMeshPath();
	std::string GetTexturePath();
	void SetName(const char* newName);
	ModuleComponents* GetComponent(COMPONENT_TYPES type);
	ModuleComponents* CreateComponent(COMPONENT_TYPES type);

public:

	std::vector<ModuleComponents*>	components;
	std::vector<ModuleGameObject*>	childs;
	
	ModuleGameObject* parent;
private:

	uint id;
	std::string name;
	bool is_active;
	bool is_static;
	bool selectedForInspector = false;
};

#endif // __ModuleGameObject_H__