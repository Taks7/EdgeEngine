#include "Inspector.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"

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
	ModuleGameObject* selectedGameObject = App->scene_intro->GetSelectedGameObject();

	ImGui::Begin("Inspector");
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetWindowPos({ float(App->window->GetWidht() - 300),20});
		ImGui::SetWindowSize({ 300,550 });
		
		
		
		if (selectedGameObject != nullptr && selectedGameObject != App->scene_intro->rootObject)
		{
			DrawInfoOfGameObject(selectedGameObject);
			DrawGameObjectComponents(selectedGameObject);
		}
	}
	ImGui::End();

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
	
	ImGui::SameLine();
	bool BoundingBoxActive = game_object->IsBBActive();
	if (ImGui::Checkbox("Bounding box", &BoundingBoxActive));
	{
		game_object->SetBB(BoundingBoxActive);
	}

}

void InspectorMenu::DrawGameObjectComponents(ModuleGameObject* selectedGameObject)
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
					DrawMeshComponent(selectedGameObject);
					break;
				}
				case COMPONENT_TYPES::MATERIAL:
				{
					DrawMaterialComponent(selectedGameObject);
					break;
				}
				case COMPONENT_TYPES::TRANSFORM:
				{
					DrawTransformComponent(selectedGameObject);
					break;
				}
				case COMPONENT_TYPES::CAMERA:
				{
					DrawCameraComponent(selectedGameObject);
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

void InspectorMenu::DrawTransformComponent(ModuleGameObject* selectedGameObject)
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

			float3 rotation = transform->GetLocalEulerRotation() * RADTODEG;
			float rot[3] = { rotation.x, rotation.y, rotation.z };
			if (ImGui::DragFloat3("Rotation", rot, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetRotation(float3(rot[0] * DEGTORAD, rot[1] * DEGTORAD, rot[2] * DEGTORAD));
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

void InspectorMenu::DrawMeshComponent(ModuleGameObject* selectedGameObject)
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

			if (mesh->meshPath != nullptr && ((ModuleComponentCamera*)selectedGameObject->GetComponent(COMPONENT_TYPES::CAMERA) == nullptr))
			{
				std::string meshPath = mesh->meshPath;

				ImGui::Text("File: %s", meshPath);
			}
			
			ImGui::Separator();

			ImGui::Text("Mesh Info:");

			ImGui::Text("Vertices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "  %u", mesh->mesh.num_vertex);
			ImGui::Text("Indices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", mesh->mesh.num_index);
			ImGui::Text("UVS:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", mesh->mesh.num_uvs);

			ImGui::Separator();

			if (ImGui::Button("Save mesh", { 100,100 }))
			{
				char* buffer = nullptr;
				mesh->mesh.Save(&mesh->mesh, &buffer);
			}

		}
		
	}
}

void InspectorMenu::DrawMaterialComponent(ModuleGameObject* selectedGameObject)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModuleComponentMaterial* material = (ModuleComponentMaterial*)selectedGameObject->GetComponent(COMPONENT_TYPES::MATERIAL);
		
		if (material->materialUsed != nullptr)
		{
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

				std::string texturePath= material->materialUsed->path.c_str();
				
				ImGui::Text("File: %s",texturePath.c_str());
				
				ImGui::Image(ImTextureID(material->materialUsed->id), { 128,128 });

				ImGui::Separator();

				ImGui::Text("Texture Data:");

				ImGui::Text("Width:");	ImGui::SameLine(); ImGui::Text(" %u", material->materialUsed->width);
				ImGui::Text("Height:");	ImGui::SameLine(); ImGui::Text("%u", material->materialUsed->height);

				ImGui::Separator();

				if (ImGui::Button("Save Texture", {100,100}))
				{
					char* buffer = nullptr;
					material->materialUsed->Save_texture(material->materialUsed, &buffer);
				}
			}		
		}		
	}
}

void InspectorMenu::DrawCameraComponent(ModuleGameObject* selectedGameObject)
{
	if (ImGui::CollapsingHeader("Camera View Selector", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModuleComponentCamera* camera = (ModuleComponentCamera*)selectedGameObject->GetComponent(COMPONENT_TYPES::CAMERA);

		

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Camera Selector:");

		if (ImGui::Button("Set Game Camera"))
		{
			App->camera->SetCurrentCamera(camera);
		}

		if (ImGui::Button("Return to Editor View Camera"))
		{
			App->camera->SetMasterCameraAsCurrentCamera();
		}

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Camera Settings:");

		bool camera_is_culling = camera->IsCulling();
		if (ImGui::Checkbox("Culling", &camera_is_culling))
		{
			camera->SetIsCulling(camera_is_culling);
		}

		bool camera_is_orthogonal = camera->OrthogonalView();
		if (ImGui::Checkbox("Select Orthogonal", &camera_is_orthogonal))
		{
			camera->SetOrthogonalView(camera_is_orthogonal);
		}

		bool frustum_is_hidden = camera->FrustumIsHidden();
		if (ImGui::Checkbox("Hide Frustum", &frustum_is_hidden))
		{
			camera->SetFrustumIsHidden(frustum_is_hidden);
		}

		ImGui::Separator();

		float near_plane_distance = camera->GetNearPlaneDistance();
		if (ImGui::SliderFloat("Near Plane", &near_plane_distance, 0.1f, 1000.0f, "%.3f", 0))
		{
			camera->SetNearPlaneDistance(near_plane_distance);
		}

		float far_plane_distance = camera->GetFarPlaneDistance();
		if (ImGui::SliderFloat("Far Plane", &far_plane_distance, 0.1f, 1000.0f, "%.3f", 0))
		{
			camera->SetFarPlaneDistance(far_plane_distance);
		}

		int fov = (int)camera->GetVerticalFOV();
		uint min_fov = 0;
		uint max_fov = 0;
		camera->GetMinMaxFOV(min_fov, max_fov);
		if (ImGui::SliderInt("FOV", &fov, min_fov, max_fov, "%d"))
		{
			camera->SetVerticalFOV((float)fov);
		}

		
	}
}