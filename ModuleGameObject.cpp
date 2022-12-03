#include "Globals.h"
#include "Application.h"
#include "ModuleGameObject.h"
#include "ModuleComponent.h"

#include "ModuleComponentMaterial.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentCamera.h"

ModuleGameObject::ModuleGameObject(uint id, std::string name,bool isActive, bool isStatic) : id(id),name(name), is_active(isActive), is_static(isStatic) 
,parent(nullptr)
{
	name = "GameObject";
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

	UpdateBoundingBoxes();

	return true;
}

// Called before quitting
bool ModuleGameObject::CleanUp()
{
	return true;
}

void ModuleGameObject::Render()
{
	for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
	{
		ModuleComponentsTransform* transform = (ModuleComponentsTransform*)App->scene_intro->game_objects.at(i)->GetComponent(COMPONENT_TYPES::TRANSFORM);
		App->renderer3D->DrawGameObjects(*App->scene_intro->game_objects.at(i), transform->GetGlobalMatrix());
		ModuleGameObject* owner = App->scene_intro->game_objects.at(i);
		for (int j = 0; j < owner->childs.size(); j++)
		{
			ModuleComponentsTransform* transform_childs = (ModuleComponentsTransform*)owner->childs.at(j)->GetComponent(COMPONENT_TYPES::TRANSFORM);
			App->renderer3D->DrawGameObjects(*owner->childs.at(j),transform_childs->GetGlobalMatrix());
		}
	}

}

ModuleComponents* ModuleGameObject::CreateComponent(COMPONENT_TYPES type)
{
	ModuleComponents* component = nullptr;

	bool findDuplicates = false;

	switch (type)
	{
	case COMPONENT_TYPES::TRANSFORM:
		component = new ModuleComponentsTransform(this);
		findDuplicates = true;
		break;

	case COMPONENT_TYPES::MESH:
		component = new ModuleComponentsMesh(this);
		findDuplicates = true;
		break;

	case COMPONENT_TYPES::MATERIAL:
		component = new ModuleComponentMaterial(this);
		findDuplicates = true;
		break;

	case COMPONENT_TYPES::CAMERA:
		component = new ModuleComponentCamera(this);
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
void ModuleGameObject::SetActive(bool state)
{
	is_active = state;

	for (int j = 0; j < childs.size(); j++)
	{
		childs[j]->SetActive(state);
	}
}
bool ModuleGameObject::IsStatic()
{
	return is_static;


}

void ModuleGameObject::SetStatic(bool state)
{
	is_static = state;

	for (int j = 0; j < childs.size(); j++)
	{
		childs[j]->SetStatic(state);
	}
}

std::string ModuleGameObject::GetName()
{
	return name;
}

void ModuleGameObject::SetName(const char* newName)
{
	name = newName;
}

bool ModuleGameObject::IsSelected()
{
	return selectedForInspector;
}

bool ModuleGameObject::IsBBActive()
{
	return bouindingBoxes;
}

void ModuleGameObject::SetBB(bool state)
{
	bouindingBoxes = state;

	for (int j = 0; j < childs.size(); j++)
	{
		childs[j]->SetBB(state);
	}
}

void ModuleGameObject::UpdateBoundingBoxes()
{

	for (uint i = 0; i < App->scene_intro->game_objects.size(); ++i)
	{
		ModuleComponentsMesh* mesh = (ModuleComponentsMesh*)App->scene_intro->game_objects.at(i)->GetComponent(COMPONENT_TYPES::MESH);
		if (mesh == nullptr)
		{
			continue;
		}
		obb = mesh->mesh.GetAABB();
		ModuleComponentsTransform* transform = (ModuleComponentsTransform*)this->GetComponent(COMPONENT_TYPES::TRANSFORM);
		obb.Transform(transform->GetGlobalMatrix());

		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);
	}
}

bool ModuleGameObject::AddChild(ModuleGameObject* child)
{
	child->parent = this;

	childs.push_back(child);
	
	return true;
}

bool ModuleGameObject::DeleteChild(ModuleGameObject* child)
{
	bool ret = false;

	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i] == child)
		{
			childs.erase(childs.begin() + i);
			ret = true;
			break;
		}
	}

	return ret;
}

std::string ModuleGameObject::GetMeshPath()
{
	for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
	{
		if (App->scene_intro->game_objects[i]->IsSelected())
		{
			ModuleComponentsMesh* meshUsed = (ModuleComponentsMesh*)App->scene_intro->game_objects[i]->GetComponent(COMPONENT_TYPES::MESH);

			return meshUsed->meshPath;
		}
	}
}

std::string ModuleGameObject::GetTexturePath()
{
	for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
	{
		if (App->scene_intro->game_objects[i]->IsSelected())
		{
			ModuleComponentMaterial* materialUsed = (ModuleComponentMaterial*)App->scene_intro->game_objects[i]->GetComponent(COMPONENT_TYPES::MATERIAL);

			return materialUsed->materialUsed->path;
		}
	}
}

