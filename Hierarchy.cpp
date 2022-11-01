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
		ImGui::SetWindowPos({ 0,20 });
		ImGui::SetWindowSize({ 300,550});
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
			
			if (App->scene_intro->game_objects[i]->childs.empty())
			{
				ImGui::MenuItem(App->scene_intro->game_objects[i]->GetName().c_str());
				{
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						App->scene_intro->game_objects[i]->SelectItem();
					}
				}
			}
			
			if (!App->scene_intro->game_objects[i]->childs.empty())
			{
				ImGui::MenuItem(App->scene_intro->game_objects[i]->GetName().c_str());
				{
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						App->scene_intro->game_objects[i]->SelectItem();
					}

					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						PopUpOptions(App->scene_intro->game_objects[i]);
					}
					//Por ahora dejamos esto comentado que no workea del todo bien //
					/*
					for (int j = 0; j < App->scene_intro->game_objects[i]->childs.size(); j++)
					{
						ImGui::MenuItem(App->scene_intro->game_objects[i]->childs.at(j)->GetName().c_str());
					}
					//ImGui::TreePop();
					*/
				}
				
			}

		}
	}

	
}

void Hierarchy::PopUpOptions(ModuleGameObject* gameObject)
{
	ImGui::Begin("Tools");

	if(ImGui::BeginChild("Hierarchy Tools"))
	{
		if (ImGui::MenuItem("Delete Selected"))
		{
			
		}

		if (ImGui::MenuItem("Create Empty Child GameObject"))
		{

		}

		ImGui::EndChild();
	}
	ImGui::End();
}
