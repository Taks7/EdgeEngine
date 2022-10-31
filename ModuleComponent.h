#ifndef __ModuleComponent_H__
#define __ModuleComponent_H__

#include "Module.h"
#include "ModuleGameObject.h"
#include <string>

class ModuleGameObject;

enum class COMPONENT_TYPES
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
	UNKNOWN = -1
};

class ModuleComponents : public Module
{
public:
	ModuleComponents(ModuleGameObject* owner, COMPONENT_TYPES type, const char* name, bool is_active = true);
	virtual ~ModuleComponents();

	virtual bool	Update();
	virtual bool	CleanUp();

public:
	const char*     GetName() const;
	void			SetName(const char* new_name);

	bool			IsActive() const;
	void			SetIsActive(const bool& is_active);

public:
	COMPONENT_TYPES	type;

	ModuleGameObject* owner;

private:
	const char* name;
	bool is_active;
};

#endif // !__Component_H__