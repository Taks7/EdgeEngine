#include "Inspector.h"
#include "ImGui/imgui.h"
#include "Application.h"
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
	ImGui::Begin("Inspector");
	{
		ImGui::SetWindowPos({ float(App->window->screen_surface->h + 180),20 });
		ImGui::SetWindowSize({ 300,550 });
	
	}
	ImGui::End();

}