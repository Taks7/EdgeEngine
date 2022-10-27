#include "Globals.h"
#include "Application.h"
#include "ModuleGameObject.h"
#include "ModuleComponent.h"

#include "ModuleComponentMaterial.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentMesh.h"

ModuleGameObject::ModuleGameObject(uint id, std::string name,bool isActive, bool isStatic) : id(id),name(name), is_active(isActive), is_static(isStatic)
{
	name = "GameObject";
	CreateComponent(COMPONENT_TYPES::MESH);
}

// Destructor
ModuleGameObject::~ModuleGameObject()
{

}
bool ModuleGameObject::Update()
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsActive())
		{
			components[i]->Update();
		}
	}
	return true;
}

// Called before quitting
bool ModuleGameObject::CleanUp()
{
	return true;
}


ModuleComponents* ModuleGameObject::CreateComponent(COMPONENT_TYPES type)
{
	ModuleComponents* component = nullptr;

	bool findDuplicates = false;

	switch (type)
	{
	case COMPONENT_TYPES::TRANSFORM:
		//component = new (this)ModuleComponentsTransform; // Add
		//findDuplicates = true;
		break;

	case COMPONENT_TYPES::MESH:
		component = new ModuleComponentsMesh(this);
		break;

	case COMPONENT_TYPES::MATERIAL:
	/*	component = new ModuleComponentMaterial(this);
		findDuplicates = true;*/
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

ModuleComponents* ModuleGameObject::GetComponent(COMPONENT_TYPES type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
		{
			return components[i];
		}
	}

	return nullptr;
}
bool ModuleGameObject::IsActive()
{
	return is_active;
}

bool ModuleGameObject::IsStatic()
{
	return is_static;
}

std::string ModuleGameObject::GetName()
{
	return name;
}
