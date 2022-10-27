#include "Hierarchy.h"
#include "ImGui/imgui.h"
Hierarchy::Hierarchy()
{
	active = true;
}

Hierarchy::~Hierarchy()
{
	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
}

void Hierarchy::Draw()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Hierarchy");
	{
		ImGui::SetWindowPos({ float(App->window->screen_surface->h - 130),20 });
		ImGui::SetWindowSize({ 600,700 });
		GameObjectList();
	}
	ImGui::End();

}

void Hierarchy::GameObjectList()
{
	if (!App->scene_intro->game_objects.empty())
	{
		for (uint i = 0; i < App->scene_intro->game_objects.size(); i++)
		{
			ImGui::MenuItem(App->scene_intro->game_objects[i]->GetName().c_str());

		}
	}
}

void Hierarchy::PopUpOptions()
{
	ImGui::BeginPopup("Options");
	{
		if (ImGui::MenuItem("Delete Game Object"))
		{

		}
	}
	ImGui::EndPopup();
}