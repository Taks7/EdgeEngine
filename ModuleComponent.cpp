
#include "Globals.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleGameObject.h"

ModuleComponents::ModuleComponents(ModuleGameObject* owner, COMPONENT_TYPES type, const char* name, bool is_active) :
type(type),
owner(owner),
name(name),
is_active(is_active)
{

}

ModuleComponents::~ModuleComponents()
{

}

bool ModuleComponents::Update()
{
	bool ret = true;
	
	
	return ret;
}

bool ModuleComponents::CleanUp()
{
	bool ret = true;

	return ret;
}

const char* ModuleComponents::GetName() const
{
	return name;
}

void ModuleComponents::SetName(const char* new_name)
{
	name = new_name;
}

bool ModuleComponents::IsActive() const
{
	return is_active;
}

void ModuleComponents::SetIsActive(const bool& set_to)
{
	is_active = set_to;

	for (int j = 0; j < owner->childs.size(); j++)
	{
		for (int i = 0; i < owner->components.size(); i++)
		{
			owner->childs[j]->components[i]->SetIsActive(owner->components[i]->IsActive());
		}
	}
}

