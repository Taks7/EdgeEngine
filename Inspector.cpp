#include "Inspector.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentTransform.h"

InspectorMenu::InspectorMenu()
{
	active = true;
}

InspectorMenu::~InspectorMenu()
{
	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
}

void InspectorMenu::Draw()
{
	selectedGameObject = GetSelectedGameObject();

	ImGui::Begin("Inspector");
	{
		ImGui::SetWindowPos({ float(App->window->screen_surface->h + 180),20 });
		ImGui::SetWindowSize({ 300,550 });
		
		
		
		if (selectedGameObject != nullptr)
		{
			DrawInfoOfGameObject(selectedGameObject);
			DrawGameObjectComponents();
		}
	}
	ImGui::End();

}

ModuleGameObject* InspectorMenu::GetSelectedGameObject()
{
	ModuleGameObject* selectedGameObject;
	for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
	{
		if (App->scene_intro->game_objects[i]->IsSelected())
		{
			selectedGameObject = App->scene_intro->game_objects[i];
			return selectedGameObject;
		}
	}

	return nullptr;
}
void InspectorMenu::DrawInfoOfGameObject(ModuleGameObject* game_object)
{
	bool gameObjectActive = game_object->IsActive();
	if(ImGui::Checkbox("Active", &gameObjectActive));
	{
		game_object->SetActive(gameObjectActive);
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.33f);
	static char buffer[64];
	strcpy(buffer, game_object->GetName().c_str());
	if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		game_object->SetName(buffer);
	}

	bool gameObjectStatic = true;
	if (ImGui::Checkbox("Is Static", &gameObjectStatic))
	{
		game_object->SetStatic(gameObjectStatic);
	}
	
	ImGui::Separator();

}

void InspectorMenu::DrawGameObjectComponents()
{
	for (uint i = 0; i < selectedGameObject->components.size(); i++)
	{
		ModuleComponents* components = selectedGameObject->components[i];

		if (components != nullptr)
		{
			switch (components->type)
			{
				case COMPONENT_TYPES::MESH:
				{
					DrawMeshComponent();
					break;
				}
				case COMPONENT_TYPES::MATERIAL:
				{
					DrawMaterialComponent();
					break;
				}
				case COMPONENT_TYPES::TRANSFORM:
				{
					DrawTransformComponent();
					break;
				}

			}

			if (components->type == COMPONENT_TYPES::NONE || components->type == COMPONENT_TYPES::UNKNOWN)
			{
				LOG_COMMENT("[WARNING] Components invalid");
			}
		}
	}
}

void InspectorMenu::DrawTransformComponent()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
	}
}

void InspectorMenu::DrawMeshComponent()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModuleComponentsMesh* mesh = (ModuleComponentsMesh*)selectedGameObject->GetComponent(COMPONENT_TYPES::MESH);

		if (mesh != nullptr)
		{
			bool active = mesh->IsActive();
			if (ImGui::Checkbox("Mesh Active", &active))
			{
				mesh->SetIsActive(active);
			}
		}
	}
}

void InspectorMenu::DrawMaterialComponent()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModuleComponentMaterial* material = (ModuleComponentMaterial*)selectedGameObject->GetComponent(COMPONENT_TYPES::MATERIAL);

		if (material != nullptr)
		{
			bool active = material->IsActive();
			if (ImGui::Checkbox("Material Active", &active))
			{
				material->SetIsActive(active);
			}
		}
	}
}