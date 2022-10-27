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
	bool IsStatic();
	ModuleComponents* CreateComponent(COMPONENT_TYPES type);

public:

	std::vector<ModuleComponents*>	components;
	std::vector<ModuleGameObject*>	childs;

private:

	uint id;
	std::string name;
	bool is_active;
	bool is_static;
};

#endif // __ModuleGameObject_H__