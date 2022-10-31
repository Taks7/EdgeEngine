#include "Inspector.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

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

	ImGui::Text(game_object->GetName().c_str());
}