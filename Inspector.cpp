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
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetWindowPos({ float(App->window->GetWidht() - 300),20});
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
		ModuleComponentsTransform* transform = (ModuleComponentsTransform*)selectedGameObject->GetComponent(COMPONENT_TYPES::TRANSFORM);

		if (transform != nullptr)
		{
			bool active = transform->IsActive();
			if (ImGui::Checkbox("Transform Active", &active))
			{
				transform->SetIsActive(active);
			}

			ImGui::Separator();

			ImGui::Text("Position");

			ImGui::SameLine(100.0f);

			float3 position = transform->GetPosition();
			float pos[3] = { position.x, position.y, position.z };
			if (ImGui::DragFloat3("Position", pos, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetPosition(float3(pos[0], pos[1], pos[2]));
			}

			ImGui::Text("Rotation");

			ImGui::SameLine(100.0f);

			float3 rotation = transform->GetRotation();
			float rot[3] = { rotation.x, rotation.y, rotation.z };
			if (ImGui::DragFloat3("Rotation", rot, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetRotation(float3(rot[0], rot[1], rot[2]));
			}

			ImGui::Text("Scale");

			ImGui::SameLine(100.0f);

			float3 scale = transform->GetScale();
			float scl[3] = { scale.x, scale.y, scale.z };
			if (ImGui::DragFloat3("Scale", scl, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetScale(float3(scl[0], scl[1], scl[2]));
			}
		}
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

			ImGui::Separator();

			ImGui::Text("File path:");

			ImGui::SameLine();

			ImGui::Text("WIP");
			ImGui::Separator();

			ImGui::Text("Mesh Info:");

			ImGui::Text("Vertices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "  %u", mesh->mesh.num_vertex);
			ImGui::Text("Indices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", mesh->mesh.num_index);
			ImGui::Text("UVS:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", mesh->mesh.num_uvs);

			ImGui::Separator();


		}
		
	}
}

void InspectorMenu::DrawMaterialComponent()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModuleComponentMaterial* material = (ModuleComponentMaterial*)selectedGameObject->GetComponent(COMPONENT_TYPES::MATERIAL);

		if (material->materialUsed != nullptr)
		{
			bool active = material->IsActive();
			if (ImGui::Checkbox("Material Active", &active))
			{
				material->SetIsActive(active);
			}

			if (ImGui::Checkbox("Checker Texture", &App->renderer3D->checkerTextureApplied))
			{
				App->renderer3D->LoadCheckerTexture();
			}

			ImGui::Separator();

		
			ImGui::Text("File:");		
			ImGui::SameLine(); 
			ImGui::Text("WIP");

			ImGui::Separator();

			ImGui::Text("Texture Data:");

			ImGui::Text("Width:");	ImGui::SameLine(); ImGui::Text(" %u", material->materialUsed->width);
			ImGui::Text("Height:");	ImGui::SameLine(); ImGui::Text("%u", material->materialUsed->height);

			ImGui::Separator();
		}
		
	}
}