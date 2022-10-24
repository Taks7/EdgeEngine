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

class ModuleGameObject : public Module
{
public:

	ModuleGameObject(bool start_enabled = true);

	// Destructor
	virtual ~ModuleGameObject();

	bool Init();
	bool Update(float dt);
	void Render();
	bool CleanUp();

	ModuleComponents* CreateComponent(COMPONENT_TYPES type);

public:

	std::vector<ModuleComponents*>	components;
	std::vector<ModuleGameObject*>	childs;

private:

	uint id;
	std::string name;
};

#endif // __ModuleGameObject_H__