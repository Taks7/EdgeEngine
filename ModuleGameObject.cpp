#include "Globals.h"
#include "Application.h"
#include "ModuleGameObject.h"
#include "ModuleComponent.h"

ModuleGameObject::ModuleGameObject(bool start_enabled) : Module(start_enabled)
{
	name = "GameObject";
}

// Destructor
ModuleGameObject::~ModuleGameObject()
{

}

// Called before render is available
bool ModuleGameObject::Init()
{
	bool ret = true;

	return true;
}

bool ModuleGameObject::Update(float dt)
{
	return true;
}

// Called before quitting
bool ModuleGameObject::CleanUp()
{
	return true;
}

/*
ModuleComponents* ModuleGameObject::CreateComponent(COMPONENT_TYPES type)
{
	ModuleComponents* component = nullptr;

	bool findDuplicates = false;

	switch (type)
	{
	case COMPONENT_TYPES::TRANSFORM:
		component = new transformComponent(this); // Add
		findDuplicates = true;
		break;

	case COMPONENT_TYPES::MESH:
		component = new meshComponent(this);
		break;

	case COMPONENT_TYPES::MATERIAL:
		component = new materialComponent(this);
		findDuplicates = true;
		break;

	}

	if (component != nullptr)
	{
		if (findDuplicates)
		{
			for (uint i = 0; i < components.size(); ++i)
			{
				if (type == components[i]->type)
				{
					LOG_COMMENT("[ERROR] %s Component could not be added to %s: No duplicates allowed!", component->GetName(), name.c_str());

					delete component;
					component = nullptr;

					return nullptr;
				}
			}
		}

		components.push_back(component);
	}

	return component;
}
*/
